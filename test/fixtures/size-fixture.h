/**
 * @file size-fixture.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Test fixture for size.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#include "base/empty-base-fixture.h"
#include "base/constants.h"

template <typename Obj_>
class SizeFixture : public EmptyBaseFixture<Obj_> {
protected:
};

TYPED_TEST_SUITE_P(SizeFixture);

TYPED_TEST_P(SizeFixture, SizeWhileEmpty) {
    EXPECT_EQ(this->obj_.size(), 0);
}

TYPED_TEST_P(SizeFixture, SizeIncreasing) {
    using key_type = typename TypeParam::key_type;

    auto lower_bound = this->obj_.lower_bound();
    auto upper_bound = this->obj_.upper_bound();
    
    auto last_size = this->obj_.size();
    
    int insert_count = 0;
    for (key_type key = lower_bound; (key < upper_bound) && (insert_count < MAX_KEYS); ++key) {
        this->obj_.insert(key);
        EXPECT_GT(this->obj_.size(), last_size) << insert_count;
        last_size = this->obj_.size();
        ++insert_count;
    }
}

TYPED_TEST_P(SizeFixture, SizeNondecreasing) {
    auto last_size = this->obj_.size();
    for (int insert_count = 0; insert_count < MAX_KEYS; ++insert_count) {
        auto key = this->random_key();
        this->obj_.insert(key);
        EXPECT_LE(last_size, this->obj_.size());
    }
}

TYPED_TEST_P(SizeFixture, SizeDecreasing) {
    using key_type = typename TypeParam::key_type;

    auto lower_bound = this->obj_.lower_bound();
    auto upper_bound = this->obj_.upper_bound();
    
    int insert_count = 0;
    for (key_type key = lower_bound; (key < upper_bound) && (insert_count < MAX_KEYS); ++key) {
        this->obj_.insert(key);
        ++insert_count;
    }

    auto last_size = this->obj_.size();
    int remove_count = 0;
    for (key_type key = lower_bound; (key < upper_bound) && (remove_count < MAX_KEYS); ++key) {
        this->obj_.remove(key);
        EXPECT_LT(this->obj_.size(), last_size);
        last_size = this->obj_.size();
        ++remove_count;
    }

    EXPECT_EQ(this->obj_.size(), 0);
}

TYPED_TEST_P(SizeFixture, SizeInsertTwice) {
    using key_type = typename TypeParam::key_type;

    auto lower_bound = this->obj_.lower_bound();
    auto upper_bound = this->obj_.upper_bound();
    
    int insert_count = 0;
    for (key_type key = lower_bound; (key < upper_bound) && (insert_count < MAX_KEYS); ++key) {
        this->obj_.insert(key);
        auto size = this->obj_.size();
        this->obj_.insert(key);
        EXPECT_EQ(this->obj_.size(), size);
        ++insert_count;
    }
}

TYPED_TEST_P(SizeFixture, SizeRemoveTwice) {
    using key_type = typename TypeParam::key_type;

    auto lower_bound = this->obj_.lower_bound();
    auto upper_bound = this->obj_.upper_bound();
    
    int remove_count = 0;
    for (key_type key = lower_bound; (key < upper_bound) && (remove_count < MAX_KEYS); ++key) {
        this->obj_.remove(key);
        auto size = this->obj_.size();
        this->obj_.remove(key);
        EXPECT_EQ(this->obj_.size(), size);
        ++remove_count;
    }
}

REGISTER_TYPED_TEST_SUITE_P(SizeFixture,
    SizeWhileEmpty, SizeIncreasing, SizeNondecreasing, SizeDecreasing, SizeInsertTwice, 
    SizeRemoveTwice);
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(SizeFixture);
