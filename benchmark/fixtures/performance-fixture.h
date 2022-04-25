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
class PerformanceFixture : public EmptyBaseFixture<Obj_> { 
protected:
    PerformanceFixture() {
        this->Iterations(1);
    }

    virtual void BM_performance(benchmark::State& state) {
        for (auto _ : state) {
            Obj_ obj;
            std::vector<typename Obj_::key_type> keys;
            benchmark::DoNotOptimize(obj);

            std::srand(0);
            for (int i = 0; i < state.range(0); ++i) {
                auto val = std::rand() % obj.upper_bound();
                if (obj.contains(val) == false) {
                    obj.insert(val);
                    keys.push_back(val);
                }
                benchmark::ClobberMemory();
            }

            for (int i = 0; i < state.range(0); ++i) {
                auto val = std::rand() % obj.upper_bound();
                benchmark::DoNotOptimize(obj.contains(val));
                benchmark::ClobberMemory();
            }

            for (auto val : keys) {
                obj.remove(val);
                benchmark::ClobberMemory();
            }
        }
    }
};