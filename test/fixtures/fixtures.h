/**
 * @file fixtures.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Helpful macros for specializing text fixtures.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#pragma once
#include <cstdint>
#include "size-fixture.h"
#include "contains-fixture.h"
#include "predecessor-fixture.h"
#include "successor-fixture.h"
#include "min-fixture.h"
#include "max-fixture.h"
#include "insert-fixture.h"
#include "remove-fixture.h"

#define DEFINE_OBJ_TYPES(OBJ_TYPE) \
    using ObjTypes = testing::Types<OBJ_TYPE<uint8_t>, OBJ_TYPE<uint16_t>, OBJ_TYPE<uint32_t>, OBJ_TYPE<uint64_t>>;

#define DEFINE_SIZE_TESTS(OBJ_TYPE) \
    DEFINE_OBJ_TYPES(OBJ_TYPE); \
    INSTANTIATE_TYPED_TEST_SUITE_P(OBJ_TYPE, SizeFixture, ObjTypes);

#define DEFINE_CONTAINS_TESTS(OBJ_TYPE) \
    DEFINE_OBJ_TYPES(OBJ_TYPE); \
    INSTANTIATE_TYPED_TEST_SUITE_P(OBJ_TYPE, ContainsFixture, ObjTypes);

#define DEFINE_PREDECESSOR_TESTS(OBJ_TYPE) \
    DEFINE_OBJ_TYPES(OBJ_TYPE); \
    INSTANTIATE_TYPED_TEST_SUITE_P(OBJ_TYPE, PredecessorFixture, ObjTypes);

#define DEFINE_SUCCESSOR_TESTS(OBJ_TYPE) \
    DEFINE_OBJ_TYPES(OBJ_TYPE); \
    INSTANTIATE_TYPED_TEST_SUITE_P(OBJ_TYPE, SuccessorFixture, ObjTypes);

#define DEFINE_MIN_TESTS(OBJ_TYPE) \
    DEFINE_OBJ_TYPES(OBJ_TYPE); \
    INSTANTIATE_TYPED_TEST_SUITE_P(OBJ_TYPE, MinFixture, ObjTypes);

#define DEFINE_MAX_TESTS(OBJ_TYPE) \
    DEFINE_OBJ_TYPES(OBJ_TYPE); \
    INSTANTIATE_TYPED_TEST_SUITE_P(OBJ_TYPE, MaxFixture, ObjTypes);

#define DEFINE_INSERT_TESTS(OBJ_TYPE) \
    DEFINE_OBJ_TYPES(OBJ_TYPE); \
    INSTANTIATE_TYPED_TEST_SUITE_P(OBJ_TYPE, InsertFixture, ObjTypes);

#define DEFINE_REMOVE_TESTS(OBJ_TYPE) \
    DEFINE_OBJ_TYPES(OBJ_TYPE); \
    INSTANTIATE_TYPED_TEST_SUITE_P(OBJ_TYPE, RemoveFixture, ObjTypes);

#define TEST_MAIN()