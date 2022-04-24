/**
 * @file test.cpp
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#include "../fixtures/fixtures.h"
#include "../../src/red-black-tree/red-black-tree.h"

DEFINE_SIZE_TESTS(RedBlackTree);
DEFINE_CONTAINS_TESTS(RedBlackTree);
DEFINE_PREDECESSOR_TESTS(RedBlackTree);
DEFINE_SUCCESSOR_TESTS(RedBlackTree);
DEFINE_MIN_TESTS(RedBlackTree);
DEFINE_MAX_TESTS(RedBlackTree);
DEFINE_INSERT_TESTS(RedBlackTree);
DEFINE_REMOVE_TESTS(RedBlackTree);
TEST_MAIN();