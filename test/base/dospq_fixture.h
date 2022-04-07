#include <gtest/gtest.h>
#include <cstdint>
#include <random>
#include "constants.h"


#define REGISTER_DOSPQ_TEST_SUITE(DOSPQ_TYPE) \
template <typename T> \
class DOSPQTestFixture : public testing::Test { \
protected: \
	static void SetUpTestSuite() { \
		if (keys_ptr == nullptr) { \
			keys_ptr = new std::vector<T>(); \
			std::mt19937_64 mte; \
			std::uniform_int_distribution<T> dist(std::numeric_limits<T>::min(), \
												std::numeric_limits<T>::max()); \
			while (keys_ptr->size() != KEY_COUNT && keys_ptr->size() <= std::numeric_limits<T>::max()) { \
				keys_ptr->push_back(dist(mte)); \
				std::sort(std::begin(*keys_ptr), std::end(*keys_ptr)); \
				auto new_end = std::unique(std::begin(*keys_ptr), std::end(*keys_ptr)); \
				keys_ptr->erase(new_end, std::end(*keys_ptr)); \
			} \
			std::random_shuffle(std::begin(*keys_ptr), std::end(*keys_ptr)); \
		} \
		assert(keys_ptr->size() > 2); \
	} \
\
	static void TearDownTestSuite() { \
		delete keys_ptr; \
		keys_ptr = nullptr; \
	} \
\
	DOSPQ_TYPE<T> dospq; \
	static std::vector<T>* keys_ptr; \
}; \
\
template <class T> \
std::vector<T>* DOSPQTestFixture<T>::keys_ptr = nullptr; \

template <class T>
class DOSPQTestFixture;

typedef testing::Types<uint8_t, uint16_t, uint32_t, uint64_t> Implementations;

TYPED_TEST_SUITE(DOSPQTestFixture, Implementations);

TYPED_TEST(DOSPQTestFixture, IsEmptyOnCreation) {
	EXPECT_EQ(this->dospq.size(), 0);
}

TYPED_TEST(DOSPQTestFixture, HasCorrectLimit) {
	EXPECT_EQ(this->dospq.upper_bound(), std::numeric_limits<TypeParam>::max());
}

TYPED_TEST(DOSPQTestFixture, ContainsWorksOnEmpty) {
	for (const auto key : (*this->keys_ptr))
		EXPECT_EQ(this->dospq.contains(key), false);
}

TYPED_TEST(DOSPQTestFixture, RemoveWorksOnEmpty) {
	for (const auto key : (*this->keys_ptr)) {
		this->dospq.remove(key);
		EXPECT_EQ(this->dospq.size(), 0);
	}
}

TYPED_TEST(DOSPQTestFixture, MinWorksOnEmpty) {
	EXPECT_EQ(this->dospq.min().has_value(), false);
}

TYPED_TEST(DOSPQTestFixture, MaxWorksOnEmpty) {
	EXPECT_EQ(this->dospq.max().has_value(), false);
}

TYPED_TEST(DOSPQTestFixture, PredecessorWorksOnEmpty) {
	for (const auto key : (*this->keys_ptr))
		EXPECT_EQ(this->dospq.predecessor(key).has_value(), false);
}

TYPED_TEST(DOSPQTestFixture, SuccessorWorksOnEmpty) {
	for (const auto key : (*this->keys_ptr))
		EXPECT_EQ(this->dospq.successor(key).has_value(), false);
}

TYPED_TEST(DOSPQTestFixture, InsertAndCheckValues) {
	for (const auto key : (*this->keys_ptr)) {
		this->dospq.insert(key);
		EXPECT_EQ(this->dospq.contains(key), true);
	}
}

TYPED_TEST(DOSPQTestFixture, DoubleInsertIsNoop) {
	for (const auto key : (*this->keys_ptr)) {
		this->dospq.insert(key);
		auto size = this->dospq.size();
		EXPECT_EQ(this->dospq.contains(key), true);
		this->dospq.insert(key);
		EXPECT_EQ(this->dospq.size(), size);
		EXPECT_EQ(this->dospq.contains(key), true);
	}
}

TYPED_TEST(DOSPQTestFixture, InsertAndRemove) {
	for (const auto key : (*this->keys_ptr))
		this->dospq.insert(key);
	
	for (const auto key : (*this->keys_ptr)) {
		this->dospq.remove(key);
		EXPECT_EQ(this->dospq.contains(key), false);
	}
}

TYPED_TEST(DOSPQTestFixture, TestPredecessorAndSuccessor) {
	for (const auto key : (*this->keys_ptr))
		this->dospq.insert(key);
	
	std::sort(std::begin((*this->keys_ptr)), std::end((*this->keys_ptr)));

	auto predecessor = this->dospq.predecessor((*this->keys_ptr)[0]);
	auto successor = this->dospq.successor((*this->keys_ptr)[0]);

	EXPECT_EQ(predecessor.has_value(), false);
	ASSERT_EQ(successor.has_value(), true);
	EXPECT_EQ(successor.value(), (*this->keys_ptr)[1]);

	for (size_t i = 1; i < (*this->keys_ptr).size() - 1; ++i) {
		predecessor = this->dospq.predecessor((*this->keys_ptr)[i]);
		successor = this->dospq.successor((*this->keys_ptr)[i]);

		ASSERT_EQ(predecessor.has_value(), true);
		EXPECT_EQ(predecessor.value(), (*this->keys_ptr)[i - 1]);
		ASSERT_EQ(successor.has_value(), true);
		EXPECT_EQ(successor.value(), (*this->keys_ptr)[i + 1]);
	}

	predecessor = this->dospq.predecessor((*this->keys_ptr)[(*this->keys_ptr).size() - 1]);
	successor = this->dospq.successor((*this->keys_ptr)[(*this->keys_ptr).size() - 1]);

	ASSERT_EQ(predecessor.has_value(), true);
	EXPECT_EQ(predecessor.value(), (*this->keys_ptr)[(*this->keys_ptr).size() - 2]);
	EXPECT_EQ(successor.has_value(), false);
}

TYPED_TEST(DOSPQTestFixture, InsertAndRemoveProbe) {
	for (const auto key : (*this->keys_ptr))
		this->dospq.insert(key);
	
	for (const auto key : (*this->keys_ptr)) {
		auto new_key = this->dospq.upper_bound() - key;
		this->dospq.remove(new_key);
		EXPECT_EQ(this->dospq.contains(new_key), false);
		this->dospq.insert(new_key);
		EXPECT_EQ(this->dospq.contains(new_key), true);
		this->dospq.remove(new_key);
		EXPECT_EQ(this->dospq.contains(new_key), false);
	}
}