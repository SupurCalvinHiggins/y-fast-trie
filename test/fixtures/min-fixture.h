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

REGISTER_TYPED_TEST_SUITE_P(MinFixture,
    MinEmpty);
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(MinFixture);
