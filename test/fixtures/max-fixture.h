/**
 * @file max-fixture.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Test fixture for max.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#include "base/empty-base-fixture.h"
#include "base/constants.h"

template <typename Obj_>
class MaxFixture : public EmptyBaseFixture<Obj_> {
protected:
};

TYPED_TEST_SUITE_P(MaxFixture);

TYPED_TEST_P(MaxFixture, MaxEmpty) {
    EXPECT_FALSE(this->obj_.max().has_value());
}

TYPED_TEST_P(MaxFixture, MaxLowerBound) {
    auto lower_bound = this->obj_.lower_bound();
    this->obj_.insert(lower_bound);
    ASSERT_TRUE(this->obj_.max().has_value());
    EXPECT_EQ(this->obj_.max().value(), lower_bound);
}

TYPED_TEST_P(MaxFixture, MaxUpperBound) {
    auto upper_bound = this->obj_.upper_bound();
    this->obj_.insert(upper_bound);
    ASSERT_TRUE(this->obj_.max().has_value());
    EXPECT_EQ(this->obj_.max().value(), upper_bound);
}

REGISTER_TYPED_TEST_SUITE_P(MaxFixture,
    MaxEmpty, MaxLowerBound, MaxUpperBound);
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(MaxFixture);