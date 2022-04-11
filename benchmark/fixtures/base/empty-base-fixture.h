#pragma once
#include <benchmark/benchmark.h>

template <typename Obj_>
class EmptyBaseFixture : public benchmark::Fixture {
protected:
    void SetUp(const benchmark::State& state) override {
        std::srand(0);
    }
};