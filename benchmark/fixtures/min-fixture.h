#pragma once
#include "base/full-base-fixture.h"

template <typename Obj_>
class MinFixture : public FullBaseFixture<Obj_> {
protected: 
    virtual void BM_min(benchmark::State& state) {
        for (auto _ : state) {
			benchmark::DoNotOptimize(this->obj_.min());
        }
    }
};