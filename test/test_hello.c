/* Unit tests for hello.c
 * Tests the greeting functionality using a simple testing framework
 * 
 * Contract for get_greeting():
 * - Returns a pointer to a static string constant
 * - The returned pointer remains valid for the lifetime of the program
 * - Caller must NOT free() the returned pointer
 * - The returned string is read-only and should not be modified
 * - Multiple calls return the same static pointer
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
    TEST_ASSERT_EQUAL_STRING("Ciao, Mondo!", greeting);
}

/* Test that the greeting has the correct length */
void test_get_greeting_length(void) {
    const char* greeting = get_greeting();
    int expected_length = 13; /* "Ciao, Mondo!" is 13 characters */
    int actual_length = (int)strlen(greeting);
    TEST_ASSERT_EQUAL_INT(expected_length, actual_length);
}

/* Test that the greeting doesn't contain a newline */
void test_get_greeting_no_newline(void) {
    const char* greeting = get_greeting();
    const char* newline_pos = strchr(greeting, '\n');
    /* strchr returns NULL if character not found, which is what we want */
    tests_run++;
    if (newline_pos == NULL) {
        tests_passed++;
        print_green("  ✓ ");
        printf("No newline test passed\n");
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("No newline test failed - greeting contains newline\n");
    }
}

/* Test that get_greeting returns static storage (pointer stable across calls) */
void test_get_greeting_static_storage(void) {
    const char* greeting1 = get_greeting();
    const char* greeting2 = get_greeting();
    
    /* Both pointers should be identical (same memory address) for static storage */
    tests_run++;
    if (greeting1 == greeting2) {
        tests_passed++;
        print_green("  ✓ ");
        printf("Static storage test passed\n");
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("Static storage test failed - pointers differ\n");
        printf("    First call:  %p\n", (void*)greeting1);
        printf("    Second call: %p\n", (void*)greeting2);
    }
}

/* Main test runner */
int main(void) {
    printf("\n");
    print_yellow("========================================\n");
    printf("Running Unit Tests for hello.c\n");
    print_yellow("========================================\n");
    printf("\n");
    
    RUN_TEST(test_get_greeting_not_null);
    RUN_TEST(test_get_greeting_returns_hello_world);
    RUN_TEST(test_get_greeting_length);
    RUN_TEST(test_get_greeting_no_newline);
    RUN_TEST(test_get_greeting_static_storage);
    
    TEST_SUMMARY();
}
