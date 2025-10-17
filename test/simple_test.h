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
 * - TTY detection for color support (set SIMPLE_TEST_NO_COLOR=1 to disable)
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
 * 
 * Color output:
 *   Colors are automatically disabled if:
 *   - Output is not a TTY (e.g., redirected to file or pipe)
 *   - Environment variable SIMPLE_TEST_NO_COLOR is set to "1"
 *   This ensures compatibility with CI systems and non-ANSI terminals.
 */
#ifndef SIMPLE_TEST_H
#define SIMPLE_TEST_H

#include <stdio.h>
#include <string.h>

/* Test statistics - extern declarations (defined in simple_test.c) */
extern int tests_run;
extern int tests_passed;
extern int tests_failed;

/* Helper function declarations (implemented in simple_test.c) */
int use_colors(void);
void print_color(const char* color_code, const char* text);
void print_green(const char* text);
void print_red(const char* text);
void print_yellow(const char* text);

/* Test assertions */
#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    do { \
        tests_run++; \
        if (strcmp((expected), (actual)) == 0) { \
            tests_passed++; \
            print_green("  ✓ "); \
            printf("String equality test passed\n"); \
        } else { \
            tests_failed++; \
            print_red("  ✗ "); \
            printf("String equality test failed\n"); \
            printf("    Expected: \"%s\"\n", (expected)); \
            printf("    Actual:   \"%s\"\n", (actual)); \
        } \
    } while(0)

#define TEST_ASSERT_NOT_NULL(ptr) \
    do { \
        tests_run++; \
        if ((ptr) != NULL) { \
            tests_passed++; \
            print_green("  ✓ "); \
            printf("Not null test passed\n"); \
        } else { \
            tests_failed++; \
            print_red("  ✗ "); \
            printf("Not null test failed\n"); \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    do { \
        tests_run++; \
        if ((expected) == (actual)) { \
            tests_passed++; \
            print_green("  ✓ "); \
            printf("Integer equality test passed\n"); \
        } else { \
            tests_failed++; \
            print_red("  ✗ "); \
            printf("Integer equality test failed\n"); \
            printf("    Expected: %d\n", (expected)); \
            printf("    Actual:   %d\n", (actual)); \
        } \
    } while(0)

/* Test runner macros */
#define RUN_TEST(test_func) \
    do { \
        print_yellow("Running "); \
        printf(#test_func "...\n"); \
        test_func(); \
    } while(0)

#define TEST_SUMMARY() \
    do { \
        printf("\n"); \
        print_yellow("========================================\n"); \
        printf("Test Summary\n"); \
        print_yellow("========================================\n"); \
        printf("Total tests:  %d\n", tests_run); \
        if (tests_failed == 0) { \
            print_green("Passed:       "); \
            printf("%d\n", tests_passed); \
            print_green("Failed:       "); \
            printf("%d\n", tests_failed); \
            printf("\n"); \
            print_green("✓ All tests passed!\n"); \
        } else { \
            print_green("Passed:       "); \
            printf("%d\n", tests_passed); \
            print_red("Failed:       "); \
            printf("%d\n", tests_failed); \
            printf("\n"); \
            print_red("✗ Some tests failed!\n"); \
        } \
        return (tests_failed == 0) ? 0 : 1; \
    } while(0)

#endif /* SIMPLE_TEST_H */
