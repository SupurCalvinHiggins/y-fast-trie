/**
 * @file max-fixture.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Benchmark fixture for max.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#pragma once
#include "base/full-base-fixture.h"

template <typename Obj_>
class MaxFixture : public FullBaseFixture<Obj_> { 
protected:
    virtual void BM_max(benchmark::State& state) {
        for (auto _ : state) {
			benchmark::DoNotOptimize(this->obj_.max());
        }
    }
};