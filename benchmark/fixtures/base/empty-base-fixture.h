/**
 * @file empty-base-fixture.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Empty base fixture for benchmarking.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#pragma once
#include <benchmark/benchmark.h>

template <typename Obj_>
class EmptyBaseFixture : public benchmark::Fixture {
protected:
    void SetUp(const benchmark::State& state) override {
        std::srand(0);
    }
};