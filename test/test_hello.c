/* Unit tests for hello.c
 * Tests the greeting functionality using a simple testing framework
 */
#include <stdio.h>
#include <string.h>
#include "simple_test.h"
#include "../hello.h"

/* Test that get_greeting returns a non-null pointer */
void test_get_greeting_not_null(void) {
    const char* greeting = get_greeting();
    TEST_ASSERT_NOT_NULL(greeting);
}

/* Test that get_greeting returns the expected string */
void test_get_greeting_returns_hello_world(void) {
    const char* greeting = get_greeting();
    TEST_ASSERT_EQUAL_STRING("Hello world!", greeting);
}

/* Test that the greeting has the correct length */
void test_get_greeting_length(void) {
    const char* greeting = get_greeting();
    int expected_length = 12; /* "Hello world!" is 12 characters */
    int actual_length = (int)strlen(greeting);
    TEST_ASSERT_EQUAL_INT(expected_length, actual_length);
}

/* Test that the greeting doesn't contain a newline */
void test_get_greeting_no_newline(void) {
    const char* greeting = get_greeting();
    const char* newline_pos = strchr(greeting, '\n');
    /* strchr returns NULL if character not found, which is what we want */
    if (newline_pos == NULL) {
        tests_run++;
        tests_passed++;
        printf(COLOR_GREEN "  ✓ " COLOR_RESET "No newline test passed\n");
    } else {
        tests_run++;
        tests_failed++;
        printf(COLOR_RED "  ✗ " COLOR_RESET "No newline test failed - greeting contains newline\n");
    }
}

/* Main test runner */
int main(void) {
    printf("\n");
    printf(COLOR_YELLOW "========================================\n");
    printf("Running Unit Tests for hello.c\n");
    printf("========================================\n" COLOR_RESET);
    printf("\n");
    
    RUN_TEST(test_get_greeting_not_null);
    RUN_TEST(test_get_greeting_returns_hello_world);
    RUN_TEST(test_get_greeting_length);
    RUN_TEST(test_get_greeting_no_newline);
    
    TEST_SUMMARY();
}
