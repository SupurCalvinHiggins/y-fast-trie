#pragma once
#include "base/empty-base-fixture.h"
#include "base/constants.h"
#include <vector>
#include <algorithm>

template <typename Obj_>
class PredecessorFixture : public EmptyBaseFixture<Obj_> {
protected:
};

TYPED_TEST_SUITE_P(PredecessorFixture);

TYPED_TEST_P(PredecessorFixture, PredecessorEmpty) {
    auto lower_bound = this->obj_.lower_bound();
    auto upper_bound = this->obj_.upper_bound();
    EXPECT_FALSE(this->obj_.predecessor(lower_bound).has_value());
    EXPECT_FALSE(this->obj_.predecessor(upper_bound).has_value());
}

TYPED_TEST_P(PredecessorFixture, PredecessorLowerBound) {
    auto lower_bound = this->obj_.lower_bound();
    this->obj_.insert(lower_bound);
    EXPECT_FALSE(this->obj_.predecessor(lower_bound).has_value());
}

TYPED_TEST_P(PredecessorFixture, PredecessorUpperBound) {
    auto upper_bound = this->obj_.upper_bound();
    this->obj_.insert(upper_bound);
    EXPECT_FALSE(this->obj_.predecessor(upper_bound).has_value());
} 

TYPED_TEST_P(PredecessorFixture, PredecessorSomeUnique) {
    using key_type = typename TypeParam::key_type;

    auto lower_bound = this->obj_.lower_bound();
    auto upper_bound = this->obj_.upper_bound();

    std::vector<key_type> keys;
    
    for (int insert_count = 0; insert_count < MAX_KEYS; ++insert_count) {
        auto key = this->random_key();
        if (this->obj_.contains(key)) continue;

        this->obj_.insert(key);
        keys.push_back(key);
        std::sort(keys.begin(), keys.end());

        int idx = 0;
        while (keys[idx++] != key) {  }
        ASSERT_GT(idx, 0);
        idx -= 1;

        if (idx == 0)
            EXPECT_FALSE(this->obj_.predecessor(key).has_value());
        else {
            ASSERT_TRUE(this->obj_.predecessor(key).has_value());
            EXPECT_EQ(this->obj_.predecessor(key).value(), keys[idx - 1]);
        }
    }
}

REGISTER_TYPED_TEST_SUITE_P(PredecessorFixture,
    PredecessorEmpty, PredecessorLowerBound, PredecessorUpperBound, PredecessorSomeUnique);
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(PredecessorFixture);
