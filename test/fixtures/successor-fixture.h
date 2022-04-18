#pragma once
#include "base/empty-base-fixture.h"
#include "base/constants.h"
#include <vector>
#include <algorithm>

template <typename Obj_>
class SuccessorFixture : public EmptyBaseFixture<Obj_> {
protected:
};

TYPED_TEST_SUITE_P(SuccessorFixture);

TYPED_TEST_P(SuccessorFixture, SuccessorEmpty) {
    auto lower_bound = this->obj_.lower_bound();
    auto upper_bound = this->obj_.upper_bound();
    EXPECT_FALSE(this->obj_.successor(lower_bound).has_value());
    EXPECT_FALSE(this->obj_.successor(upper_bound).has_value());
}

TYPED_TEST_P(SuccessorFixture, SuccessorLowerBound) {
    auto lower_bound = this->obj_.lower_bound();
    this->obj_.insert(lower_bound);
    EXPECT_FALSE(this->obj_.successor(lower_bound).has_value());
}

TYPED_TEST_P(SuccessorFixture, SuccessorUpperBound) {
    auto upper_bound = this->obj_.upper_bound();
    this->obj_.insert(upper_bound);
    EXPECT_FALSE(this->obj_.successor(upper_bound).has_value());
} 

TYPED_TEST_P(SuccessorFixture, SuccessorSomeUnique) {
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

        if (idx == keys.size() - 1)
            EXPECT_FALSE(this->obj_.successor(key).has_value());
        else {
            ASSERT_TRUE(this->obj_.successor(key).has_value());
            EXPECT_EQ(this->obj_.successor(key).value(), keys[idx + 1]);
        }
    }
}

REGISTER_TYPED_TEST_SUITE_P(SuccessorFixture,
    SuccessorEmpty, SuccessorLowerBound, SuccessorUpperBound, SuccessorSomeUnique);
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(SuccessorFixture);
