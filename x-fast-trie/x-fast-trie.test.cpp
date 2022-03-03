#include "x-fast-trie.h"
#include <gtest/gtest.h>
#include <cstdint>

class XFastTrieTest : public ::testing::Test {
protected:
	XFastTrie<uint64_t> trie64_;
	XFastTrie<uint32_t> trie32_;
	XFastTrie<uint16_t> trie16_;
	XFastTrie<uint8_t> trie8_;
	std::vector<uint64_t> vec64_{0x9162c2ef9502b62c,
								0x0ed183c7f286b882,
								0xc9a26ad137b1ec35,
								0x1fe16931d5c31bb6,
								0x7788bf0bc23c344a,
								0x80f637e26ec3bece,
								0xd06dfed0fe7ea4cc,
								0x81b36fb73a43b39d,
								0xe5abbf5a7cf263f6,
								0xcb77063e430dff9a};
	std::vector<uint32_t> vec32_{0x16c78e43,
								0xb9b2b735,
								0x2dbc0f7f,
								0xe38e14d8,
								0x2be4c684,
								0x9e298897,
								0x18c53535,
								0xce1cac5a,
								0x4aa54679,
								0x12073a29};
	std::vector<uint16_t> vec16_{0x653e,
								0xdb30,
								0x5b43,
								0x81d1,
								0xc214,
								0xe9df,
								0xf4c0,
								0x9536,
								0x5f90,
								0x34ff};
	std::vector<uint8_t> vec8_{	0x8c,
								0xe5,
								0x46,
								0x35,
								0x5f,
								0xc5,
								0xa0,
								0x1a,
								0x44,
								0xc4};
};

TEST_F(XFastTrieTest, IsEmptyOnCreation) {
	EXPECT_EQ(trie64_.size(), 0);
	EXPECT_EQ(trie32_.size(), 0);
	EXPECT_EQ(trie16_.size(), 0);
	EXPECT_EQ(trie8_.size(), 0);
}

TEST_F(XFastTrieTest, HasCorrectLimit) {
	EXPECT_EQ(trie64_.limit(), 0xFFFFFFFFFFFFFFFF);
	EXPECT_EQ(trie32_.limit(), 0xFFFFFFFF);
	EXPECT_EQ(trie16_.limit(), 0xFFFF);
	EXPECT_EQ(trie8_.limit(), 0xFF);
}

TEST_F(XFastTrieTest, ContainsWorksOnEmpty) {
	EXPECT_EQ(trie64_.contains(0), false);
	EXPECT_EQ(trie32_.contains(0), false);
	EXPECT_EQ(trie16_.contains(0), false);
	EXPECT_EQ(trie8_.contains(0), false);
}

TEST_F(XFastTrieTest, RemoveWorksOnEmpty) {
	trie64_.remove(0);
	trie32_.remove(0);
	trie16_.remove(0);
	trie8_.remove(0);
}

TEST_F(XFastTrieTest, MinWorksOnEmpty) {
	EXPECT_EQ(trie64_.min().has_value(), false);
	EXPECT_EQ(trie32_.min().has_value(), false);
	EXPECT_EQ(trie16_.min().has_value(), false);
	EXPECT_EQ(trie8_.min().has_value(), false);
}

TEST_F(XFastTrieTest, MaxWorksOnEmpty) {
	EXPECT_EQ(trie64_.max().has_value(), false);
	EXPECT_EQ(trie32_.max().has_value(), false);
	EXPECT_EQ(trie16_.max().has_value(), false);
	EXPECT_EQ(trie8_.max().has_value(), false);
}

TEST_F(XFastTrieTest, PredecessorWorksOnEmpty) {
	EXPECT_EQ(trie64_.predecessor(0xFFFFFFFFFFFFFFFF).has_value(), false);
	EXPECT_EQ(trie32_.predecessor(0xFFFFFFFF).has_value(), false);
	EXPECT_EQ(trie16_.predecessor(0xFFFF).has_value(), false);
	EXPECT_EQ(trie8_.predecessor(0xFF).has_value(), false);
}

TEST_F(XFastTrieTest, SuccessorWorksOnEmpty) {
	EXPECT_EQ(trie64_.successor(0).has_value(), false);
	EXPECT_EQ(trie32_.successor(0).has_value(), false);
	EXPECT_EQ(trie16_.successor(0).has_value(), false);
	EXPECT_EQ(trie8_.successor(0).has_value(), false);
}

TEST_F(XFastTrieTest, InsertAndCheckValues) {
	for (uint64_t val : vec64_) {
		EXPECT_EQ(trie64_.contains(val), false);
		trie64_.insert(val);
		EXPECT_EQ(trie64_.contains(val), true);
	}

	for (uint32_t val : vec32_) {
		EXPECT_EQ(trie32_.contains(val), false);
		trie32_.insert(val);
		EXPECT_EQ(trie32_.contains(val), true);
	}

	for (uint16_t val : vec16_) {
		EXPECT_EQ(trie16_.contains(val), false);
		trie16_.insert(val);
		EXPECT_EQ(trie16_.contains(val), true);
	}

	for (uint8_t val : vec8_) {
		EXPECT_EQ(trie8_.contains(val), false);
		trie8_.insert(val);
		EXPECT_EQ(trie8_.contains(val), true);
	}
}

TEST_F(XFastTrieTest, DoubleInsertIsNoop) {
	for (uint64_t val : vec64_) {
		EXPECT_EQ(trie64_.contains(val), false);
		trie64_.insert(val);
		auto size = trie64_.size();
		EXPECT_EQ(trie64_.contains(val), true);
		trie64_.insert(val);
		EXPECT_EQ(trie64_.size(), size);
		EXPECT_EQ(trie64_.contains(val), true);
	}

	for (uint32_t val : vec32_) {
		EXPECT_EQ(trie32_.contains(val), false);
		trie32_.insert(val);
		size_t size = trie32_.size();
		EXPECT_EQ(trie32_.contains(val), true);
		trie32_.insert(val);
		EXPECT_EQ(trie32_.size(), size);
		EXPECT_EQ(trie32_.contains(val), true);
	}

	for (uint16_t val : vec16_) {
		EXPECT_EQ(trie16_.contains(val), false);
		trie16_.insert(val);
		size_t size = trie16_.size();
		EXPECT_EQ(trie16_.contains(val), true);
		trie16_.insert(val);
		EXPECT_EQ(trie16_.size(), size);
		EXPECT_EQ(trie16_.contains(val), true);
	}

	for (uint8_t val : vec8_) {
		EXPECT_EQ(trie8_.contains(val), false);
		trie8_.insert(val);
		size_t size = trie8_.size();
		EXPECT_EQ(trie8_.contains(val), true);
		trie8_.insert(val);
		EXPECT_EQ(trie8_.size(), size);
		EXPECT_EQ(trie8_.contains(val), true);
	}
}

TEST_F(XFastTrieTest, InsertAndRemove) {
	for (uint64_t val : vec64_) {
		EXPECT_EQ(trie64_.contains(val), false);
		trie64_.insert(val);
		EXPECT_EQ(trie64_.contains(val), true);
	}
	EXPECT_EQ(trie64_.size(), vec64_.size());

	for (uint64_t val : vec64_) {
		EXPECT_EQ(trie64_.contains(val), true);
		trie64_.remove(val);
		EXPECT_EQ(trie64_.contains(val), false);
	}
	EXPECT_EQ(trie64_.size(), 0);

	for (uint32_t val : vec32_) {
		EXPECT_EQ(trie32_.contains(val), false);
		trie32_.insert(val);
		EXPECT_EQ(trie32_.contains(val), true);
	}
	EXPECT_EQ(trie32_.size(), vec32_.size());

	for (uint32_t val : vec32_) {
		EXPECT_EQ(trie32_.contains(val), true);
		trie32_.remove(val);
		EXPECT_EQ(trie32_.contains(val), false);
	}
	EXPECT_EQ(trie32_.size(), 0);

	for (uint16_t val : vec16_) {
		EXPECT_EQ(trie16_.contains(val), false);
		trie16_.insert(val);
		EXPECT_EQ(trie16_.contains(val), true);
	}
	EXPECT_EQ(trie16_.size(), vec16_.size());

	for (uint16_t val : vec16_) {
		EXPECT_EQ(trie16_.contains(val), true);
		trie16_.remove(val);
		EXPECT_EQ(trie16_.contains(val), false);
	}
	EXPECT_EQ(trie16_.size(), 0);

	for (uint8_t val : vec8_) {
		EXPECT_EQ(trie8_.contains(val), false);
		trie8_.insert(val);
		EXPECT_EQ(trie8_.contains(val), true);
	}
	EXPECT_EQ(trie8_.size(), vec8_.size());

	for (uint8_t val : vec8_) {
		EXPECT_EQ(trie8_.contains(val), true);
		trie8_.remove(val);
		EXPECT_EQ(trie8_.contains(val), false);
	}
	EXPECT_EQ(trie8_.size(), 0);
}

TEST_F(XFastTrieTest, TestPredecessorAndSuccessor) {
	for (uint64_t val : vec64_) {
		trie64_.insert(val);
	}

	std::sort(std::begin(vec64_), std::end(vec64_));

	EXPECT_EQ(trie64_.predecessor(vec64_[0]).has_value(), false);
	ASSERT_EQ(trie64_.successor(vec64_[0]).has_value(), true);
	EXPECT_EQ(trie64_.successor(vec64_[0]).value(), vec64_[1]);

	for (size_t i = 1; i < vec64_.size() - 1; ++i) {
		ASSERT_EQ(trie64_.predecessor(vec64_[i]).has_value(), true);
		ASSERT_EQ(trie64_.successor(vec64_[i]).has_value(), true);
		EXPECT_EQ(trie64_.predecessor(vec64_[i]).value(), vec64_[i - 1]);
		EXPECT_EQ(trie64_.successor(vec64_[i]).value(), vec64_[i + 1]);
	}

	ASSERT_EQ(trie64_.predecessor(vec64_[vec64_.size() - 1]).has_value(), true);
	EXPECT_EQ(trie64_.predecessor(vec64_[vec64_.size() - 1]).value(), vec64_[vec64_.size() - 2]);
	EXPECT_EQ(trie64_.successor(vec64_[vec64_.size() - 1]).has_value(), false);

	for (uint32_t val : vec32_) {
		trie32_.insert(val);
	}

	std::sort(std::begin(vec32_), std::end(vec32_));

	EXPECT_EQ(trie32_.predecessor(vec32_[0]).has_value(), false);
	ASSERT_EQ(trie32_.successor(vec32_[0]).has_value(), true);
	EXPECT_EQ(trie32_.successor(vec32_[0]).value(), vec32_[1]);

	for (size_t i = 1; i < vec32_.size() - 1; ++i) {
		ASSERT_EQ(trie32_.predecessor(vec32_[i]).has_value(), true);
		ASSERT_EQ(trie32_.successor(vec32_[i]).has_value(), true);
		EXPECT_EQ(trie32_.predecessor(vec32_[i]).value(), vec32_[i - 1]);
		EXPECT_EQ(trie32_.successor(vec32_[i]).value(), vec32_[i + 1]);
	}
	
	ASSERT_EQ(trie32_.predecessor(vec32_[vec32_.size() - 1]).has_value(), true);
	EXPECT_EQ(trie32_.predecessor(vec32_[vec32_.size() - 1]).value(), vec32_[vec32_.size() - 2]);
	EXPECT_EQ(trie32_.successor(vec32_[vec32_.size() - 1]).has_value(), false);

	for (uint16_t val : vec16_) {
		trie16_.insert(val);
	}

	std::sort(std::begin(vec16_), std::end(vec16_));

	EXPECT_EQ(trie16_.predecessor(vec16_[0]).has_value(), false);
	ASSERT_EQ(trie16_.successor(vec16_[0]).has_value(), true);
	EXPECT_EQ(trie16_.successor(vec16_[0]).value(), vec16_[1]);

	for (size_t i = 1; i < vec16_.size() - 1; ++i) {
		ASSERT_EQ(trie16_.predecessor(vec16_[i]).has_value(), true);
		ASSERT_EQ(trie16_.successor(vec16_[i]).has_value(), true);
		EXPECT_EQ(trie16_.predecessor(vec16_[i]).value(), vec16_[i - 1]);
		EXPECT_EQ(trie16_.successor(vec16_[i]).value(), vec16_[i + 1]);
	}
	
	ASSERT_EQ(trie16_.predecessor(vec16_[vec16_.size() - 1]).has_value(), true);
	EXPECT_EQ(trie16_.predecessor(vec16_[vec16_.size() - 1]).value(), vec16_[vec16_.size() - 2]);
	EXPECT_EQ(trie16_.successor(vec16_[vec16_.size() - 1]).has_value(), false);

	for (uint8_t val : vec8_) {
		trie8_.insert(val);
	}

	std::sort(std::begin(vec8_), std::end(vec8_));

	EXPECT_EQ(trie8_.predecessor(vec8_[0]).has_value(), false);
	ASSERT_EQ(trie8_.successor(vec8_[0]).has_value(), true);
	EXPECT_EQ(trie8_.successor(vec8_[0]).value(), vec8_[1]);

	for (size_t i = 1; i < vec8_.size() - 1; ++i) {
		ASSERT_EQ(trie8_.predecessor(vec8_[i]).has_value(), true);
		ASSERT_EQ(trie8_.successor(vec8_[i]).has_value(), true);
		EXPECT_EQ(trie8_.predecessor(vec8_[i]).value(), vec8_[i - 1]);
		EXPECT_EQ(trie8_.successor(vec8_[i]).value(), vec8_[i + 1]);
	}
	
	ASSERT_EQ(trie8_.predecessor(vec8_[vec8_.size() - 1]).has_value(), true);
	EXPECT_EQ(trie8_.predecessor(vec8_[vec8_.size() - 1]).value(), vec8_[vec8_.size() - 2]);
	EXPECT_EQ(trie8_.successor(vec8_[vec8_.size() - 1]).has_value(), false);
}