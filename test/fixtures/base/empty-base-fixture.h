#pragma once
#include <gtest/gtest.h>
#include <random>

template <typename Obj_>
class EmptyBaseFixture : public testing::Test {
protected:
    Obj_ obj_;

    using key_type = typename Obj_::key_type;

    key_type random_key() {
        std::mt19937_64 mte;
		std::uniform_int_distribution<key_type> dist(Obj_::lower_bound(), Obj_::upper_bound());
        return dist(mte);
    }
};