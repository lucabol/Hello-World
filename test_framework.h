#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Simple test framework for Hello World program */

/* Global test counters */
extern int tests_run;
extern int tests_passed;

/* Macros for testing */
#define ASSERT_STR_EQUAL(expected, actual) do { \
    tests_run++; \
    if (strcmp(expected, actual) == 0) { \
        tests_passed++; \
        printf("PASS: %s == %s\n", expected, actual); \
    } else { \
        printf("FAIL: Expected '%s', got '%s'\n", expected, actual); \
    } \
} while(0)

#define ASSERT_EQUAL(expected, actual) do { \
    tests_run++; \
    if (expected == actual) { \
        tests_passed++; \
        printf("PASS: %d == %d\n", expected, actual); \
    } else { \
        printf("FAIL: Expected %d, got %d\n", expected, actual); \
    } \
} while(0)

#define RUN_TEST(test_func) do { \
    printf("Running %s...\n", #test_func); \
    test_func(); \
    printf("\n"); \
} while(0)

#define TEST_SUMMARY() do { \
    printf("=== TEST SUMMARY ===\n"); \
    printf("Tests run: %d\n", tests_run); \
    printf("Tests passed: %d\n", tests_passed); \
    printf("Tests failed: %d\n", tests_run - tests_passed); \
    if (tests_passed == tests_run) { \
        printf("ALL TESTS PASSED!\n"); \
    } else { \
        printf("SOME TESTS FAILED!\n"); \
    } \
} while(0)

#endif /* TEST_FRAMEWORK_H */