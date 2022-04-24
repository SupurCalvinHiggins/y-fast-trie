/**
 * @file insert-fixture.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Test fixture for insert.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#include "base/empty-base-fixture.h"
#include "base/constants.h"

template <typename Obj_>
class InsertFixture : public EmptyBaseFixture<Obj_> {
protected:
};

TYPED_TEST_SUITE_P(InsertFixture);

TYPED_TEST_P(InsertFixture, InsertUpperBound) {
    auto upper_bound = this->obj_.upper_bound();
    this->obj_.insert(upper_bound);
    EXPECT_TRUE(this->obj_.contains(upper_bound));
}

TYPED_TEST_P(InsertFixture, InsertLowerBound) {
    auto lower_bound = this->obj_.lower_bound();
    this->obj_.insert(lower_bound);
    EXPECT_TRUE(this->obj_.contains(lower_bound));
}

TYPED_TEST_P(InsertFixture, InsertTwice) {
    auto lower_bound = this->obj_.lower_bound();
    this->obj_.insert(lower_bound);
    this->obj_.insert(lower_bound);
    EXPECT_TRUE(this->obj_.contains(lower_bound));  
} 

TYPED_TEST_P(InsertFixture, InsertSomeUnique) {
    using key_type = typename TypeParam::key_type;

    auto lower_bound = this->obj_.lower_bound();
    auto upper_bound = this->obj_.upper_bound();

    int insert_count = 0;
    for (key_type key = lower_bound; (key < upper_bound) && (insert_count < MAX_KEYS); ++key) {
        this->obj_.insert(key);
        EXPECT_TRUE(this->obj_.contains(key));
        ++insert_count;
    }
}

REGISTER_TYPED_TEST_SUITE_P(InsertFixture,
    InsertUpperBound, InsertLowerBound, InsertTwice, InsertSomeUnique);
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(InsertFixture);
