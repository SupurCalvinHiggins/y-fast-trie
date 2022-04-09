#pragma once
#include <benchmark/benchmark.h>

template <typename Obj_, typename Key_>
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