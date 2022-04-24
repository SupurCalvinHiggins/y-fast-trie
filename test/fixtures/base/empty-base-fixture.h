/**
 * @file empty-base-fixture.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Base text fixture initially containing no keys.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#pragma once
#include <gtest/gtest.h>
#include <random>

/**
 * @brief Base test fixture initially containing no keys.
 * 
 * @tparam Obj_ to test.
 */
template <typename Obj_>
class EmptyBaseFixture : public testing::Test {
protected:
    Obj_ obj_;

    using key_type = typename Obj_::key_type;

    /**
     * @brief Generate a random key.
     * 
     * @return random key. 
     */
    key_type random_key() {
        std::mt19937_64 mte;
		std::uniform_int_distribution<key_type> dist(Obj_::lower_bound(), Obj_::upper_bound());
        return dist(mte);
    }
};