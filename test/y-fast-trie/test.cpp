/**
 * @file test.cpp
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#include "../fixtures/fixtures.h"
#include "../../src/y-fast-trie/y-fast-trie.h"

DEFINE_SIZE_TESTS(YFastTrie);
DEFINE_CONTAINS_TESTS(YFastTrie);
DEFINE_PREDECESSOR_TESTS(YFastTrie);
DEFINE_SUCCESSOR_TESTS(YFastTrie);
DEFINE_MIN_TESTS(YFastTrie);
DEFINE_MAX_TESTS(YFastTrie);
DEFINE_INSERT_TESTS(YFastTrie);
DEFINE_REMOVE_TESTS(YFastTrie);
TEST_MAIN();