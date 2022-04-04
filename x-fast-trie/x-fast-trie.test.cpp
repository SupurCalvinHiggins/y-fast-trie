#include "x-fast-trie.h"
#include <gtest/gtest.h>
#include <cstdint>
#include <random>
#include <string>
#include <iostream>

const size_t KEY_COUNT = 256;

template <typename T>
class XFastTrieTest : public ::testing::Test {
protected:
	static void SetUpTestSuite() {
		if (keys_ptr == nullptr) {
			keys_ptr = new std::vector<T>();
			std::mt19937_64 mte;
			std::uniform_int_distribution<T> dist(std::numeric_limits<T>::min(), 
												std::numeric_limits<T>::max());
			while (keys_ptr->size() != KEY_COUNT && keys_ptr->size() <= std::numeric_limits<T>::max()) {
				keys_ptr->push_back(dist(mte));
				std::sort(std::begin(*keys_ptr), std::end(*keys_ptr));
				auto new_end = std::unique(std::begin(*keys_ptr), std::end(*keys_ptr));
				keys_ptr->erase(new_end, std::end(*keys_ptr));
			}
			std::random_shuffle(std::begin(*keys_ptr), std::end(*keys_ptr));
		}
		assert(keys_ptr->size() > 2);
	}

	static void TearDownTestSuite() {
		delete keys_ptr;
		keys_ptr = nullptr;
	}

	XFastTrie<T> trie;
	static std::vector<T>* keys_ptr;
};

template <class T>
std::vector<T>* XFastTrieTest<T>::keys_ptr = nullptr;

typedef testing::Types<uint8_t, uint16_t, uint32_t, uint64_t> Implementations;

TYPED_TEST_SUITE(XFastTrieTest, Implementations);

TYPED_TEST(XFastTrieTest, IsEmptyOnCreation) {
	EXPECT_EQ(this->trie.size(), 0);
}

TYPED_TEST(XFastTrieTest, HasCorrectLimit) {
	EXPECT_EQ(this->trie.upper_bound(), std::numeric_limits<TypeParam>::max());
}

TYPED_TEST(XFastTrieTest, ContainsWorksOnEmpty) {
	for (const auto key : (*this->keys_ptr))
		EXPECT_EQ(this->trie.contains(key), false);
}

TYPED_TEST(XFastTrieTest, RemoveWorksOnEmpty) {
	for (const auto key : (*this->keys_ptr)) {
		this->trie.remove(key);
		EXPECT_EQ(this->trie.size(), 0);
	}
}

TYPED_TEST(XFastTrieTest, MinWorksOnEmpty) {
	EXPECT_EQ(this->trie.min().has_value(), false);
}

TYPED_TEST(XFastTrieTest, MaxWorksOnEmpty) {
	EXPECT_EQ(this->trie.max().has_value(), false);
}

TYPED_TEST(XFastTrieTest, PredecessorWorksOnEmpty) {
	for (const auto key : (*this->keys_ptr))
		EXPECT_EQ(this->trie.predecessor(key).has_value(), false);
}

TYPED_TEST(XFastTrieTest, SuccessorWorksOnEmpty) {
	for (const auto key : (*this->keys_ptr))
		EXPECT_EQ(this->trie.successor(key).has_value(), false);
}

TYPED_TEST(XFastTrieTest, InsertAndCheckValues) {
	for (const auto key : (*this->keys_ptr)) {
		this->trie.insert(key);
		EXPECT_EQ(this->trie.contains(key), true);
	}
}

TYPED_TEST(XFastTrieTest, DoubleInsertIsNoop) {
	for (const auto key : (*this->keys_ptr)) {
		this->trie.insert(key);
		auto size = this->trie.size();
		EXPECT_EQ(this->trie.contains(key), true);
		this->trie.insert(key);
		EXPECT_EQ(this->trie.size(), size);
		EXPECT_EQ(this->trie.contains(key), true);
	}
}

TYPED_TEST(XFastTrieTest, InsertAndRemove) {
	for (const auto key : (*this->keys_ptr))
		this->trie.insert(key);
	
	for (const auto key : (*this->keys_ptr)) {
		this->trie.remove(key);
		EXPECT_EQ(this->trie.contains(key), false);
	}
}

TYPED_TEST(XFastTrieTest, TestPredecessorAndSuccessor) {
	for (const auto key : (*this->keys_ptr))
		this->trie.insert(key);
	
	std::sort(std::begin((*this->keys_ptr)), std::end((*this->keys_ptr)));

	auto predecessor = this->trie.predecessor((*this->keys_ptr)[0]);
	auto successor = this->trie.successor((*this->keys_ptr)[0]);

	EXPECT_EQ(predecessor.has_value(), false);
	ASSERT_EQ(successor.has_value(), true);
	EXPECT_EQ(successor.value(), (*this->keys_ptr)[1]);

	for (size_t i = 1; i < (*this->keys_ptr).size() - 1; ++i) {
		predecessor = this->trie.predecessor((*this->keys_ptr)[i]);
		successor = this->trie.successor((*this->keys_ptr)[i]);

		ASSERT_EQ(predecessor.has_value(), true);
		EXPECT_EQ(predecessor.value(), (*this->keys_ptr)[i - 1]);
		ASSERT_EQ(successor.has_value(), true);
		EXPECT_EQ(successor.value(), (*this->keys_ptr)[i + 1]);
	}

	predecessor = this->trie.predecessor((*this->keys_ptr)[(*this->keys_ptr).size() - 1]);
	successor = this->trie.successor((*this->keys_ptr)[(*this->keys_ptr).size() - 1]);

	ASSERT_EQ(predecessor.has_value(), true);
	EXPECT_EQ(predecessor.value(), (*this->keys_ptr)[(*this->keys_ptr).size() - 2]);
	EXPECT_EQ(successor.has_value(), false);
}

TYPED_TEST(XFastTrieTest, InsertAndRemoveValuedTrie) {
	for (const auto key : (*this->keys_ptr))
		this->trie.insert(key);
	
	for (const auto key : (*this->keys_ptr)) {
		auto new_key = this->trie.upper_bound() - key;
		this->trie.remove(new_key);
		EXPECT_EQ(this->trie.contains(new_key), false);
		this->trie.insert(new_key);
		EXPECT_EQ(this->trie.contains(new_key), true);
		this->trie.remove(new_key);
		EXPECT_EQ(this->trie.contains(new_key), false);
	}
}