#pragma once
#include "base/full-base-fixture.h"

template <typename Obj_>
class ContainsFixture : public FullBaseFixture<Obj_> { 
protected:
    virtual void BM_contains(benchmark::State& state) {
        for (auto _ : state) {
            auto val = std::rand() % this->obj_.upper_bound();
			benchmark::DoNotOptimize(this->obj_.contains(val));
        }
    }
};