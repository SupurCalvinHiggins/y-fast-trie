#pragma once
#include "base/full-base-fixture.h"

template <typename Obj_, typename Key_>
class MaxFixture : public FullBaseFixture<Obj_, Key_> { 
protected:
    virtual void BM_max(benchmark::State& state) {
        for (auto _ : state) {
			benchmark::DoNotOptimize(this->obj_.max());
        }
    }
};