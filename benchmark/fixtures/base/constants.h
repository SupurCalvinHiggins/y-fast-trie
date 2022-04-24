/**
 * @file constants.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Some constants for benchmarking.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#pragma once

// Step size between benchmarks.
constexpr uint64_t RANGE_MULTIPLER   = (1 << 4);

// Input size to start benchmarking from.
constexpr uint64_t RANGE_LOWER_BOUND = (1 << 4);

// Input size to end benchmarking on.
constexpr uint64_t RANGE_UPPER_BOUND = (1 << 16);