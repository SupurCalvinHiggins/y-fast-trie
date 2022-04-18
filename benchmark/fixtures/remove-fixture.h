#pragma once
#include "base/empty-base-fixture.h"

template <typename Obj_>
class RemoveFixture : public EmptyBaseFixture<Obj_> {
protected:
    virtual void BM_remove(benchmark::State& state) {
        for (auto _ : state) {
            state.PauseTiming();
            benchmark::ClobberMemory();

            Obj_ obj;
            benchmark::DoNotOptimize(obj);
            
            std::srand(0);
            for (int i = 0; i < state.range(0); ++i) {
                auto val = std::rand() % obj.upper_bound();
                obj.insert(val);
                benchmark::ClobberMemory();
            }
            
            state.ResumeTiming();
            benchmark::ClobberMemory();

            std::srand(0);
            for (int i = 0; i < state.range(0); ++i) {
                auto val = std::rand() % obj.upper_bound();
                obj.remove(val);
                benchmark::ClobberMemory();
            }
        }
    }
};