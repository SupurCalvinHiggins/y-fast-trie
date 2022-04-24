/**
 * @file insert-fixture.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Benchmark fixture for insert.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#pragma once
#include "base/empty-base-fixture.h"

template <typename Obj_>
class InsertFixture : public EmptyBaseFixture<Obj_> { 
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