/**
 * @file test.cpp
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#include "../fixtures/fixtures.h"
#include "../../src/x-fast-trie/x-fast-trie.h"

DEFINE_SIZE_TESTS(XFastTrie);
DEFINE_CONTAINS_TESTS(XFastTrie);
DEFINE_PREDECESSOR_TESTS(XFastTrie);
DEFINE_SUCCESSOR_TESTS(XFastTrie);
DEFINE_MIN_TESTS(XFastTrie);
DEFINE_MAX_TESTS(XFastTrie);
DEFINE_INSERT_TESTS(XFastTrie);
DEFINE_REMOVE_TESTS(XFastTrie);
TEST_MAIN();