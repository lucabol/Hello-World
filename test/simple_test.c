/* Simple C Testing Framework - Implementation
 * This file contains the implementation of the testing framework.
 * Link this file when building tests that use simple_test.h
 */

/* Enable POSIX features for fileno */
#define _POSIX_C_SOURCE 200809L

#include "simple_test.h"
#include <stdio.h>
#include <stdlib.h>

/* Platform-specific includes for isatty */
#ifdef _WIN32
    #include <io.h>
    #define isatty _isatty
    #define fileno _fileno
#else
    #include <unistd.h>
#endif

/* Test statistics - external linkage */
int tests_run = 0;
int tests_passed = 0;
int tests_failed = 0;

/* Internal state for color detection */
static int color_checked = 0;
static int should_use_colors = 0;

/* Initialize the test framework
 * This function should be called before running any tests.
 * It detects TTY and environment variables to determine color support.
 */
void simple_test_init(void) {
    color_checked = 0;
    should_use_colors = 0;
    tests_run = 0;
    tests_passed = 0;
    tests_failed = 0;
}

/* Check if colors should be used
 * This function performs one-time detection of color support based on:
 * 1. SIMPLE_TEST_NO_COLOR environment variable (set to "1" to disable)
 * 2. Whether stdout is a TTY
 */
int use_colors(void) {
    if (!color_checked) {
        color_checked = 1;
        
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

/* Helper function to print with or without colors */
void print_color(const char* color_code, const char* text) {
    if (use_colors()) {
        printf("%s%s\033[0m", color_code, text);
    } else {
        printf("%s", text);
    }
}

/* Color printing functions */
void print_green(const char* text) {
    print_color("\033[0;32m", text);
}

void print_red(const char* text) {
    print_color("\033[0;31m", text);
}

void print_yellow(const char* text) {
    print_color("\033[1;33m", text);
}
