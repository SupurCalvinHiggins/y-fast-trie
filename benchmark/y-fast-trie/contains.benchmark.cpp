/**
 * @file contains.benchmark.cpp
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#include "../fixtures/fixtures.h"
#include "../../src/y-fast-trie/y-fast-trie.h"

DEFINE_CONTAINS_BENCHMARKS(YFastTrie);
BENCHMARK_MAIN();