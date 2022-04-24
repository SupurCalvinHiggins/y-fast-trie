/**
 * @file insert.benchmark.cpp
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#include "../fixtures/fixtures.h"
#include "../../src/x-fast-trie/x-fast-trie.h"

DEFINE_INSERT_BENCHMARKS(XFastTrie);
BENCHMARK_MAIN();