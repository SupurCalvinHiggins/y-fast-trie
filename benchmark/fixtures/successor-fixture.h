/**
 * @file successor-fixture.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Benchmark fixture for successor.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#pragma once
#include "base/full-base-fixture.h"

template <typename Obj_>
class SuccessorFixture : public FullBaseFixture<Obj_> { 
protected:
    virtual void BM_successor(benchmark::State& state) {
        for (auto _ : state) {
            auto val = std::rand() % this->obj_.upper_bound();
			benchmark::DoNotOptimize(this->obj_.successor(val));
        }
    }
};