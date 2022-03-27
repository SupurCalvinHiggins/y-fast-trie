#include "set-wrapper.h"
#include <gtest/gtest.h>
#include <cstdint>
#include <random>

const size_t KEY_COUNT = 1000;

template <typename T>
class RedBlackTreeTest : public ::testing::Test {
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
		
		if (keys_ptr != nullptr){
			delete keys_ptr;
			keys_ptr = nullptr;
		}
		
	}

	RedBlackTree<T> tree;
	static std::vector<T>* keys_ptr;
};

template <class T>
std::vector<T>* RedBlackTreeTest<T>::keys_ptr = nullptr;

typedef testing::Types<uint8_t, uint16_t, uint32_t, uint64_t> Implementations;

TYPED_TEST_SUITE(RedBlackTreeTest, Implementations);

TYPED_TEST(RedBlackTreeTest, IsEmptyOnCreation) {
	EXPECT_EQ(this->tree.size(), 0);
}

TYPED_TEST(RedBlackTreeTest, ContainsWorksOnEmpty) {
	for (const auto key : (*this->keys_ptr))
		EXPECT_EQ(this->tree.contains(key), false);
}

TYPED_TEST(RedBlackTreeTest, RemoveWorksOnEmpty) {
	for (const auto key : (*this->keys_ptr)) {
		this->tree.remove(key);
		EXPECT_EQ(this->tree.size(), 0);
	}
}

TYPED_TEST(RedBlackTreeTest, PredecessorWorksOnEmpty) {
	for (const auto key : (*this->keys_ptr))
		EXPECT_EQ(this->tree.predecessor(key).has_value(), false);
}

TYPED_TEST(RedBlackTreeTest, SuccessorWorksOnEmpty) {
	for (const auto key : (*this->keys_ptr))
		EXPECT_EQ(this->tree.successor(key).has_value(), false);
}

TYPED_TEST(RedBlackTreeTest, InsertAndCheckValues) {
	for (const auto key : (*this->keys_ptr)) {
		this->tree.insert(key);
		EXPECT_EQ(this->tree.contains(key), true);
	}
}

TYPED_TEST(RedBlackTreeTest, DoubleInsertIsNoop) {
	for (const auto key : (*this->keys_ptr)) {
		this->tree.insert(key);
		auto size = this->tree.size();
		EXPECT_EQ(this->tree.contains(key), true);
		this->tree.insert(key);
		EXPECT_EQ(this->tree.size(), size);
		EXPECT_EQ(this->tree.contains(key), true);
	}
}

TYPED_TEST(RedBlackTreeTest, InsertAndRemove) {
	for (const auto key : (*this->keys_ptr))
		this->tree.insert(key);
	
	for (const auto key : (*this->keys_ptr)) {
		this->tree.remove(key);
		EXPECT_EQ(this->tree.contains(key), false);
	}
}

TYPED_TEST(RedBlackTreeTest, TestPredecessorAndSuccessor) {
	for (const auto key : (*this->keys_ptr))
		this->tree.insert(key);
	
	std::sort(std::begin((*this->keys_ptr)), std::end((*this->keys_ptr)));

	auto predecessor = this->tree.predecessor((*this->keys_ptr)[0]);
	auto successor = this->tree.successor((*this->keys_ptr)[0]);

	EXPECT_EQ(predecessor.has_value(), false);
	ASSERT_EQ(successor.has_value(), true);
	EXPECT_EQ(successor.value(), (*this->keys_ptr)[1]);

	for (size_t i = 1; i < (*this->keys_ptr).size() - 1; ++i) {
		predecessor = this->tree.predecessor((*this->keys_ptr)[i]);
		successor = this->tree.successor((*this->keys_ptr)[i]);

		ASSERT_EQ(predecessor.has_value(), true);
		EXPECT_EQ(predecessor.value(), (*this->keys_ptr)[i - 1]);
		ASSERT_EQ(successor.has_value(), true);
		EXPECT_EQ(successor.value(), (*this->keys_ptr)[i + 1]);
	}

	predecessor = this->tree.predecessor((*this->keys_ptr)[(*this->keys_ptr).size() - 1]);
	successor = this->tree.successor((*this->keys_ptr)[(*this->keys_ptr).size() - 1]);

	ASSERT_EQ(predecessor.has_value(), true);
	EXPECT_EQ(predecessor.value(), (*this->keys_ptr)[(*this->keys_ptr).size() - 2]);
	EXPECT_EQ(successor.has_value(), false);
}

TYPED_TEST(RedBlackTreeTest, InsertAndRemoveValuedTrie) {
	for (const auto key : (*this->keys_ptr))
		this->tree.insert(key);
	
	for (const auto key : (*this->keys_ptr)) {
		auto new_key = this->tree.limit() - key;
		this->tree.remove(new_key);
		EXPECT_EQ(this->tree.contains(new_key), false);
		this->tree.insert(new_key);
		EXPECT_EQ(this->tree.contains(new_key), true);
		this->tree.remove(new_key);
		EXPECT_EQ(this->tree.contains(new_key), false);
	}
}