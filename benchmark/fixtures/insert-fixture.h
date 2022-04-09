#pragma once
#include "base/empty-base-fixture.h"

template <typename Obj_, typename Key_>
class InsertFixture : public EmptyBaseFixture<Obj_, Key_> { 
protected:
    virtual void BM_insert(benchmark::State& state) {
        for (auto _ : state) {
            Obj_ obj;
            benchmark::DoNotOptimize(obj);

            std::srand(0);
            for (int i = 0; i < state.range(0); ++i) {
                auto val = std::rand() % obj.upper_bound();
                obj.insert(val);
                benchmark::ClobberMemory();
            }
        }
    }
};