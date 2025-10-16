/* Simple C Testing Framework - inspired by Unity
 * Lightweight testing framework with no external dependencies
 *
 * Public API:
 * - TEST_ASSERT_EQUAL_STRING(expected, actual) - Compare two strings for
 * equality
 * - TEST_ASSERT_NOT_NULL(ptr) - Verify a pointer is not NULL
 * - TEST_ASSERT_EQUAL_INT(expected, actual) - Compare two integers for equality
 * - RUN_TEST(test_func) - Execute a test function
 * - TEST_SUMMARY() - Print test summary and return exit code (0 = pass, 1 =
 * fail)
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

/* Enable POSIX features for fileno */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Platform-specific includes for isatty */
#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

/* Test statistics */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/* Check if colors should be used */
static int use_colors(void) {
    static int checked = 0;
    static int should_use_colors = 0;

    if (!checked) {
        checked = 1;

        /* Check environment variable first */
        const char* no_color = getenv("SIMPLE_TEST_NO_COLOR");
        if (no_color && no_color[0] == '1') {
            should_use_colors = 0;
            return should_use_colors;
        }

/* Check if stdout is a TTY - use STDOUT_FILENO constant */
#ifdef _WIN32
        should_use_colors = _isatty(_fileno(stdout));
#else
        should_use_colors = isatty(STDOUT_FILENO);
#endif
    }

    return should_use_colors;
}

/* Helper functions to print with or without colors */
static void print_color(const char* color_code, const char* text) {
    if (use_colors()) {
        printf("%s%s\033[0m", color_code, text);
    } else {
        printf("%s", text);
    }
}

static void print_green(const char* text) {
    print_color("\033[0;32m", text);
}

static void print_red(const char* text) {
    print_color("\033[0;31m", text);
}

static void print_yellow(const char* text) {
    print_color("\033[1;33m", text);
}

/* Test assertions */
#define TEST_ASSERT_EQUAL_STRING(expected, actual)        \
    do {                                                  \
        tests_run++;                                      \
        if (strcmp((expected), (actual)) == 0) {          \
            tests_passed++;                               \
            print_green("  ✓ ");                          \
            printf("String equality test passed\n");      \
        } else {                                          \
            tests_failed++;                               \
            print_red("  ✗ ");                            \
            printf("String equality test failed\n");      \
            printf("    Expected: \"%s\"\n", (expected)); \
            printf("    Actual:   \"%s\"\n", (actual));   \
        }                                                 \
    } while (0)

#define TEST_ASSERT_NOT_NULL(ptr)             \
    do {                                      \
        tests_run++;                          \
        if ((ptr) != NULL) {                  \
            tests_passed++;                   \
            print_green("  ✓ ");              \
            printf("Not null test passed\n"); \
        } else {                              \
            tests_failed++;                   \
            print_red("  ✗ ");                \
            printf("Not null test failed\n"); \
        }                                     \
    } while (0)

#define TEST_ASSERT_EQUAL_INT(expected, actual)       \
    do {                                              \
        tests_run++;                                  \
        if ((expected) == (actual)) {                 \
            tests_passed++;                           \
            print_green("  ✓ ");                      \
            printf("Integer equality test passed\n"); \
        } else {                                      \
            tests_failed++;                           \
            print_red("  ✗ ");                        \
            printf("Integer equality test failed\n"); \
            printf("    Expected: %d\n", (expected)); \
            printf("    Actual:   %d\n", (actual));   \
        }                                             \
    } while (0)

/* Test runner macros */
#define RUN_TEST(test_func)         \
    do {                            \
        print_yellow("Running ");   \
        printf(#test_func "...\n"); \
        test_func();                \
    } while (0)

#define TEST_SUMMARY()                                              \
    do {                                                            \
        printf("\n");                                               \
        print_yellow("========================================\n"); \
        printf("Test Summary\n");                                   \
        print_yellow("========================================\n"); \
        printf("Total tests:  %d\n", tests_run);                    \
        if (tests_failed == 0) {                                    \
            print_green("Passed:       ");                          \
            printf("%d\n", tests_passed);                           \
            print_green("Failed:       ");                          \
            printf("%d\n", tests_failed);                           \
            printf("\n");                                           \
            print_green("✓ All tests passed!\n");                   \
        } else {                                                    \
            print_green("Passed:       ");                          \
            printf("%d\n", tests_passed);                           \
            print_red("Failed:       ");                            \
            printf("%d\n", tests_failed);                           \
            printf("\n");                                           \
            print_red("✗ Some tests failed!\n");                    \
        }                                                           \
        return (tests_failed == 0) ? 0 : 1;                         \
    } while (0)

#endif /* SIMPLE_TEST_H */
