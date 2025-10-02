/* Simple C Testing Framework - inspired by Unity
 * Lightweight testing framework with no external dependencies
 * 
 * Public API:
 * - TEST_ASSERT_EQUAL_STRING(expected, actual) - Compare two strings for equality
 * - TEST_ASSERT_NOT_NULL(ptr) - Verify a pointer is not NULL
 * - TEST_ASSERT_EQUAL_INT(expected, actual) - Compare two integers for equality
 * - RUN_TEST(test_func) - Execute a test function
 * - TEST_SUMMARY() - Print test summary and return exit code (0 = pass, 1 = fail)
 * 
 * Features:
 * - Colored output for test results (green=pass, red=fail, yellow=info)
 * - Automatic test counting and statistics
 * - Standard exit codes for CI integration
 * 
 * Usage:
 *   #include "simple_test.h"
 *   
 *   void test_example(void) {
 *       TEST_ASSERT_EQUAL_INT(5, 2 + 3);
 *   }
 *   
 *   int main(void) {
 *       RUN_TEST(test_example);
 *       TEST_SUMMARY();
 *   }
 */
#ifndef SIMPLE_TEST_H
#define SIMPLE_TEST_H

#include <stdio.h>
#include <string.h>

/* Test statistics */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/* Color codes for output */
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_RESET "\033[0m"

/* Test assertions */
#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    do { \
        tests_run++; \
        if (strcmp((expected), (actual)) == 0) { \
            tests_passed++; \
            printf(COLOR_GREEN "  ✓ " COLOR_RESET "String equality test passed\n"); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "  ✗ " COLOR_RESET "String equality test failed\n"); \
            printf("    Expected: \"%s\"\n", (expected)); \
            printf("    Actual:   \"%s\"\n", (actual)); \
        } \
    } while(0)

#define TEST_ASSERT_NOT_NULL(ptr) \
    do { \
        tests_run++; \
        if ((ptr) != NULL) { \
            tests_passed++; \
            printf(COLOR_GREEN "  ✓ " COLOR_RESET "Not null test passed\n"); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "  ✗ " COLOR_RESET "Not null test failed\n"); \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    do { \
        tests_run++; \
        if ((expected) == (actual)) { \
            tests_passed++; \
            printf(COLOR_GREEN "  ✓ " COLOR_RESET "Integer equality test passed\n"); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "  ✗ " COLOR_RESET "Integer equality test failed\n"); \
            printf("    Expected: %d\n", (expected)); \
            printf("    Actual:   %d\n", (actual)); \
        } \
    } while(0)

/* Test runner macros */
#define RUN_TEST(test_func) \
    do { \
        printf(COLOR_YELLOW "Running " COLOR_RESET #test_func "...\n"); \
        test_func(); \
    } while(0)

#define TEST_SUMMARY() \
    do { \
        printf("\n" COLOR_YELLOW "========================================\n"); \
        printf("Test Summary\n"); \
        printf("========================================" COLOR_RESET "\n"); \
        printf("Total tests:  %d\n", tests_run); \
        if (tests_failed == 0) { \
            printf(COLOR_GREEN "Passed:       %d\n" COLOR_RESET, tests_passed); \
            printf(COLOR_GREEN "Failed:       %d\n" COLOR_RESET, tests_failed); \
            printf(COLOR_GREEN "\n✓ All tests passed!\n" COLOR_RESET); \
        } else { \
            printf(COLOR_GREEN "Passed:       %d\n" COLOR_RESET, tests_passed); \
            printf(COLOR_RED "Failed:       %d\n" COLOR_RESET, tests_failed); \
            printf(COLOR_RED "\n✗ Some tests failed!\n" COLOR_RESET); \
        } \
        return (tests_failed == 0) ? 0 : 1; \
    } while(0)

#endif /* SIMPLE_TEST_H */
