#pragma once
#include <benchmark/benchmark.h>
#include "constants.h"

template <typename DOSPQ_, typename Key_>
class FullDOSPQFixture : public benchmark::Fixture {
protected:
    void SetUp(const benchmark::State& state) override {
        std::srand(0);
        for (int i = 0; i < state.range(0); ++i) {
            auto val = std::rand() % dospq.upper_bound();
            dospq.insert(val);
        }
    }

    virtual void BM_FullDOSPQ_contains(benchmark::State& state) {
        for (auto _ : state) {
            auto val = std::rand() % dospq.upper_bound();
			benchmark::DoNotOptimize(dospq.contains(val));
        }
    }

    virtual void BM_FullDOSPQ_predecessor(benchmark::State& state) {
        for (auto _ : state) {
            auto val = std::rand() % dospq.upper_bound();
			benchmark::DoNotOptimize(dospq.predecessor(val));
        }
    }

    virtual void BM_FullDOSPQ_successor(benchmark::State& state) {
        for (auto _ : state) {
            auto val = std::rand() % dospq.upper_bound();
			benchmark::DoNotOptimize(dospq.successor(val));
        }
    }

    virtual void BM_FullDOSPQ_min(benchmark::State& state) {
        for (auto _ : state) {
			benchmark::DoNotOptimize(dospq.min());
        }
    }

    virtual void BM_FullDOSPQ_max(benchmark::State& state) {
        for (auto _ : state) {
			benchmark::DoNotOptimize(dospq.max());
        }
    }

    DOSPQ_ dospq;
};

#define REGISTER_FULL_DOSPQ_BENCHMARK(DOSPQ_TYPE, KEY_TYPE, METHOD) \
BENCHMARK_TEMPLATE_DEFINE_F(FullDOSPQFixture, BM_ ## METHOD ## _ ## KEY_TYPE, DOSPQ_TYPE<KEY_TYPE>, KEY_TYPE) (benchmark::State& state) { \
	FullDOSPQFixture::BM_FullDOSPQ_ ## METHOD(state); \
} \
BENCHMARK_REGISTER_F(FullDOSPQFixture, BM_ ## METHOD ## _ ## KEY_TYPE)->RangeMultiplier(RANGE_MULTIPLER)->Range(RANGE_LOWER_BOUND, RANGE_UPPER_BOUND); \

