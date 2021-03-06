/**
 * @file fixtures.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Some helpful macros for specializing benchmark fixtures.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#pragma once
#include <cstdint>
#include "base/constants.h"
#include "contains-fixture.h"
#include "predecessor-fixture.h"
#include "successor-fixture.h"
#include "min-fixture.h"
#include "max-fixture.h"
#include "insert-fixture.h"
#include "remove-fixture.h"
#include "performance-fixture.h"

#define DEFINE_BENCHMARK_HELPER(OBJ_TYPE, KEY_TYPE, FIXTURE_NAME, METHOD_NAME) \
BENCHMARK_TEMPLATE_DEFINE_F(FIXTURE_NAME, METHOD_NAME ## _ ## KEY_TYPE, OBJ_TYPE<KEY_TYPE>) (benchmark::State& state) { \
	FIXTURE_NAME::METHOD_NAME(state); \
} \
BENCHMARK_REGISTER_F(FIXTURE_NAME, METHOD_NAME ## _ ## KEY_TYPE)->RangeMultiplier(RANGE_MULTIPLER)->Range(RANGE_LOWER_BOUND, RANGE_UPPER_BOUND);

#define DEFINE_BENCHMARKS_HELPER(OBJ_TYPE, FIXTURE_NAME, METHOD_NAME) \
    DEFINE_BENCHMARK_HELPER(OBJ_TYPE, uint64_t, FIXTURE_NAME, METHOD_NAME) \
    DEFINE_BENCHMARK_HELPER(OBJ_TYPE, uint32_t, FIXTURE_NAME, METHOD_NAME) \
    DEFINE_BENCHMARK_HELPER(OBJ_TYPE, uint16_t, FIXTURE_NAME, METHOD_NAME) \
    DEFINE_BENCHMARK_HELPER(OBJ_TYPE, uint8_t, FIXTURE_NAME, METHOD_NAME);
    
#define DEFINE_CONTAINS_BENCHMARKS(OBJ_TYPE) \
    DEFINE_BENCHMARKS_HELPER(OBJ_TYPE, ContainsFixture, BM_contains);

#define DEFINE_PREDECESSOR_BENCHMARKS(OBJ_TYPE) \
    DEFINE_BENCHMARKS_HELPER(OBJ_TYPE, PredecessorFixture, BM_predecessor);

#define DEFINE_SUCCESSOR_BENCHMARKS(OBJ_TYPE) \
    DEFINE_BENCHMARKS_HELPER(OBJ_TYPE, SuccessorFixture, BM_successor);

#define DEFINE_MIN_BENCHMARKS(OBJ_TYPE) \
    DEFINE_BENCHMARKS_HELPER(OBJ_TYPE, MinFixture, BM_min);

#define DEFINE_MAX_BENCHMARKS(OBJ_TYPE) \
    DEFINE_BENCHMARKS_HELPER(OBJ_TYPE, MaxFixture, BM_max);

#define DEFINE_INSERT_BENCHMARKS(OBJ_TYPE) \
    DEFINE_BENCHMARKS_HELPER(OBJ_TYPE, InsertFixture, BM_insert);

#define DEFINE_REMOVE_BENCHMARKS(OBJ_TYPE) \
    DEFINE_BENCHMARKS_HELPER(OBJ_TYPE, RemoveFixture, BM_remove);

#define DEFINE_PERFORMANCE_BENCHMARKS(OBJ_TYPE) \
    DEFINE_BENCHMARK_HELPER(OBJ_TYPE, uint64_t, PerformanceFixture, BM_performance);
