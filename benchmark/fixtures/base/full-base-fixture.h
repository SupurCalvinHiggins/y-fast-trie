/**
 * @file full-base-fixture.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Base fixture with initial values.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#pragma once
#include <benchmark/benchmark.h>

template <typename Obj_>
class FullBaseFixture : public benchmark::Fixture {
protected:
    void SetUp(const benchmark::State& state) override {
        std::srand(0);
        for (int i = 0; i < state.range(0); ++i) {
            auto val = std::rand() % obj_.upper_bound();
            obj_.insert(val);
        }
    }
    Obj_ obj_;
};