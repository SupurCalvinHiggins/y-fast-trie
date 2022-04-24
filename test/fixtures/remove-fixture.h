/**
 * @file remove-fixture.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Test fixture for remove.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#include "base/empty-base-fixture.h"
#include "base/constants.h"

template <typename Obj_>
class RemoveFixture : public EmptyBaseFixture<Obj_> {
protected:
};

TYPED_TEST_SUITE_P(RemoveFixture);

TYPED_TEST_P(RemoveFixture, RemoveEmpty) {
    auto lower_bound = this->obj_.lower_bound();
    this->obj_.insert(lower_bound);
    EXPECT_TRUE(this->obj_.contains(lower_bound));  
    this->obj_.remove(lower_bound);
    EXPECT_FALSE(this->obj_.contains(lower_bound)); 
    this->obj_.remove(lower_bound);
    EXPECT_FALSE(this->obj_.contains(lower_bound));   
} 

TYPED_TEST_P(RemoveFixture, RemoveUpperBound) {
    auto upper_bound = this->obj_.upper_bound();
    this->obj_.insert(upper_bound);
    EXPECT_TRUE(this->obj_.contains(upper_bound));
    this->obj_.remove(upper_bound);
    EXPECT_FALSE(this->obj_.contains(upper_bound));
}

TYPED_TEST_P(RemoveFixture, RemoveLowerBound) {
    auto lower_bound = this->obj_.lower_bound();
    this->obj_.insert(lower_bound);
    EXPECT_TRUE(this->obj_.contains(lower_bound));
    this->obj_.remove(lower_bound);
    EXPECT_FALSE(this->obj_.contains(lower_bound));
}

TYPED_TEST_P(RemoveFixture, RemoveTwice) {
    using key_type = typename TypeParam::key_type;

    auto lower_bound = this->obj_.lower_bound();
    auto upper_bound = this->obj_.upper_bound();

    int insert_count = 0;
    for (key_type key = lower_bound; (key < upper_bound) && (insert_count < MAX_KEYS); ++key) {
        this->obj_.insert(key);
        EXPECT_TRUE(this->obj_.contains(key));
        ++insert_count;
    }

    int remove_count = 0;
    for (key_type key = lower_bound; (key < upper_bound) && (remove_count < MAX_KEYS); ++key) {
        this->obj_.remove(key);
        EXPECT_FALSE(this->obj_.contains(key));
        this->obj_.remove(key);
        EXPECT_FALSE(this->obj_.contains(key));
        ++remove_count;
    }
}

TYPED_TEST_P(RemoveFixture, RemoveSomeUnique) {
    using key_type = typename TypeParam::key_type;

    auto lower_bound = this->obj_.lower_bound();
    auto upper_bound = this->obj_.upper_bound();

    int insert_count = 0;
    for (key_type key = lower_bound; (key < upper_bound) && (insert_count < MAX_KEYS); ++key) {
        this->obj_.insert(key);
        EXPECT_TRUE(this->obj_.contains(key));
        ++insert_count;
    }

    int remove_count = 0;
    for (key_type key = lower_bound; (key < upper_bound) && (remove_count < MAX_KEYS); ++key) {
        this->obj_.remove(key);
        EXPECT_FALSE(this->obj_.contains(key));
        ++remove_count;
    }
}

REGISTER_TYPED_TEST_SUITE_P(RemoveFixture,
    RemoveEmpty, RemoveUpperBound, RemoveLowerBound, RemoveTwice, RemoveSomeUnique);
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(RemoveFixture);
