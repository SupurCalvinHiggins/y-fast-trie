#include "red-black-tree.h"
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
		delete keys_ptr;
		keys_ptr = nullptr;
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

TYPED_TEST(RedBlackTreeTest, HasCorrectLimit) {
	EXPECT_EQ(this->tree.limit(), std::numeric_limits<TypeParam>::max());
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

TYPED_TEST(RedBlackTreeTest, MinWorksOnEmpty) {
	EXPECT_EQ(this->tree.min().has_value(), false);
}

TYPED_TEST(RedBlackTreeTest, MaxWorksOnEmpty) {
	EXPECT_EQ(this->tree.max().has_value(), false);
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

TYPED_TEST(RedBlackTreeTest, TestPredecessorAndSuccessor) {
	for (const auto key : (*this->keys_ptr))
		this->tree.insert(key);
	
	std::sort(std::begin((*this->keys_ptr)), std::end((*this->keys_ptr)));

	ASSERT_EQ(tree.check_balance(),true);
}

TYPED_TEST(RedBlackTreeTest, TestBalance) {
	for (const auto key : (*this->keys_ptr))
		this->tree.insert(key);
	
	std::sort(std::begin((*this->keys_ptr)), std::end((*this->keys_ptr)));

	ASSERT_EQ(tree.check_balance(),true);
}

TYPED_TEST(RedBlackTreeTest, TestEqualBlackInPaths) {
	for (const auto key : (*this->keys_ptr))
		this->tree.insert(key);
	
	std::sort(std::begin((*this->keys_ptr)), std::end((*this->keys_ptr)));

	std::vector<Node<T>*> bottom = tree.get_layer(tree.height() - 1);
    Node<T>* node = bottom[1];
    unsigned int black_height = 0;

    while (node != nullptr) {
        if (node->color_ == 0) {
            black_height++;
        }
        node = node->parent_;
    }

    for (int i = 1; i < bottom.size(); i++){
        unsigned int black_nodes = 0;
        node = bottom[i];
        while (node != nullptr) {
            if (node->color_ == 0) {
                black_nodes++;
            }
        }
        node = node->parent_;
        ASSERT_EQ(black_nodes, black_height);
    }

}

TYPED_TEST(RedBlackTreeTest, TestNoConsequetiveReds) {
	for (const auto key : (*this->keys_ptr))
		this->tree.insert(key);
	
	std::sort(std::begin((*this->keys_ptr)), std::end((*this->keys_ptr)));

	std::vector<std::vector<Node<T>*> layers = tree.make_2d_vector();
    
    for (std::vector<Node<T>* layer : layers) {
        for(Node<T> node : layer) {
            if (node->color_) {
                EXPECT_EQ(node->parent_->color_, false);
                EXPECT_EQ(node->children_[0]->color_, false);
                EXPECT_EQ(node->children_[1]->color_, false);
            } 
        }
    }

}


    
