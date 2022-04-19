#include "base/empty-base-fixture.h"
#include "base/constants.h"

template <typename Obj_>
class MinFixture : public EmptyBaseFixture<Obj_> {
protected:
};

TYPED_TEST_SUITE_P(MinFixture);

TYPED_TEST_P(MinFixture, MinEmpty) {
    EXPECT_FALSE(this->obj_.min().has_value());
}

TYPED_TEST_P(MinFixture, MinLowerBound) {
    auto lower_bound = this->obj_.lower_bound();
    this->obj_.insert(lower_bound);
    ASSERT_TRUE(this->obj_.min().has_value());
    EXPECT_EQ(this->obj_.min().value(), lower_bound);
}

TYPED_TEST_P(MinFixture, MinUpperBound) {
    auto upper_bound = this->obj_.upper_bound();
    this->obj_.insert(upper_bound);
    ASSERT_TRUE(this->obj_.min().has_value());
    EXPECT_EQ(this->obj_.min().value(), upper_bound);
}

REGISTER_TYPED_TEST_SUITE_P(MinFixture,
    MinEmpty, MinLowerBound, MinUpperBound);
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(MinFixture);
