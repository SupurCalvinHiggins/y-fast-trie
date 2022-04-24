/**
 * @file predecessor-fixture.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Benchmark fixture for predecessor.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#pragma once
#include "base/full-base-fixture.h"

template <typename Obj_>
class PredecessorFixture : public FullBaseFixture<Obj_> { 
protected:
    virtual void BM_predecessor(benchmark::State& state) {
        for (auto _ : state) {
            auto val = std::rand() % this->obj_.upper_bound();
			benchmark::DoNotOptimize(this->obj_.predecessor(val));
        }
    }
};