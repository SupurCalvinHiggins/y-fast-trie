#pragma once
#include "base/empty-base-fixture.h"
#include <vector>

template <typename Obj_>
class RemoveFixture : public EmptyBaseFixture<Obj_> {
protected:
    virtual void BM_remove(benchmark::State& state) {
        for (auto _ : state) {
            state.PauseTiming();
            benchmark::ClobberMemory();

            Obj_ obj;
            std::vector<typename Obj_::key_type> inserted;
            benchmark::DoNotOptimize(obj);
            
            std::srand(0);
            for (int i = 0; i < state.range(0); ++i) {
                auto val = std::rand() % obj.upper_bound();
                inserted.push_back(val);
                obj.insert(val);
                benchmark::ClobberMemory();
            }
            
            state.ResumeTiming();
            benchmark::ClobberMemory();

            std::srand(0);
            for (auto val : inserted) {
                obj.remove(val);
                benchmark::ClobberMemory();
            }
        }
    }
};