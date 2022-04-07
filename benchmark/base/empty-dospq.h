#pragma once
#include <benchmark/benchmark.h>
#include "constants.h"

template <typename DOSPQ_, typename Key_>
class EmptyDOSPQFixture : public benchmark::Fixture {
protected:
    void SetUp(const benchmark::State& state) override {
        std::srand(0);
    }

    virtual void BM_EmptyDOSPQ_insert(benchmark::State& state) {
        for (auto _ : state) {
            DOSPQ_ dospq;
            benchmark::DoNotOptimize(dospq);

            std::srand(0);
            for (int i = 0; i < state.range(0); ++i) {
                auto val = std::rand() % dospq.upper_bound();
                dospq.insert(val);
                benchmark::ClobberMemory();
            }
        }
    }

    virtual void BM_EmptyDOSPQ_remove(benchmark::State& state) {
        for (auto _ : state) {
            state.PauseTiming();
            benchmark::ClobberMemory();

            DOSPQ_ dospq;
            benchmark::DoNotOptimize(dospq);
            
            std::srand(0);
            for (int i = 0; i < state.range(0); ++i) {
                auto val = std::rand() % dospq.upper_bound();
                dospq.insert(val);
                benchmark::ClobberMemory();
            }
            
            state.ResumeTiming();
            benchmark::ClobberMemory();

            std::srand(0);
            for (int i = 0; i < state.range(0); ++i) {
                auto val = std::rand() % dospq.upper_bound();
                dospq.remove(val);
                benchmark::ClobberMemory();
            }
        }
    }
};

#define REGISTER_EMPTY_DOSPQ_BENCHMARK(DOSPQ_TYPE, KEY_TYPE, METHOD) \
BENCHMARK_TEMPLATE_DEFINE_F(EmptyDOSPQFixture, BM_ ## METHOD ## _ ## KEY_TYPE, DOSPQ_TYPE<KEY_TYPE>, KEY_TYPE) (benchmark::State& state) { \
	EmptyDOSPQFixture::BM_EmptyDOSPQ_ ## METHOD(state); \
} \
BENCHMARK_REGISTER_F(EmptyDOSPQFixture, BM_ ## METHOD ## _ ## KEY_TYPE)->RangeMultiplier(RANGE_MULTIPLER)->Range(RANGE_LOWER_BOUND, RANGE_UPPER_BOUND); \

