#include "base/empty-base-fixture.h"
#include "base/constants.h"

template <typename Obj_>
class ContainsFixture : public EmptyBaseFixture<Obj_> {
protected:
};

TYPED_TEST_SUITE_P(ContainsFixture);

TYPED_TEST_P(ContainsFixture, ContainsWhileEmpty) {
    auto lower_bound = this->obj_.lower_bound();
    auto upper_bound = this->obj_.upper_bound();
    auto middle = lower_bound + ((upper_bound - lower_bound) >> 1);

    EXPECT_FALSE(this->obj_.contains(lower_bound));
    EXPECT_FALSE(this->obj_.contains(upper_bound));
    EXPECT_FALSE(this->obj_.contains(middle));
}

REGISTER_TYPED_TEST_SUITE_P(ContainsFixture,
    ContainsWhileEmpty);
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(ContainsFixture);
