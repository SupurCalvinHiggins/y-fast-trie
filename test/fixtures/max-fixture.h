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

REGISTER_TYPED_TEST_SUITE_P(MaxFixture,
    MaxEmpty);
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(MaxFixture);