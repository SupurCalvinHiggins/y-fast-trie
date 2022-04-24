/**
 * @file min-fixture.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Benchmark fixture for min.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

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