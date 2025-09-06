#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hello.h"

// Simple test framework
static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) \
    printf("Running test: %s\n", #name); \
    tests_run++; \
    if (test_##name()) { \
        printf("  PASS\n"); \
        tests_passed++; \
    } else { \
        printf("  FAIL\n"); \
    }

// Test function for get_greeting()
int test_get_greeting_returns_hello_world() {
    const char* result = get_greeting();
    return strcmp(result, "Hello world!\n") == 0;
}

int test_get_greeting_not_null() {
    const char* result = get_greeting();
    return result != NULL;
}

int test_get_greeting_not_empty() {
    const char* result = get_greeting();
    return strlen(result) > 0;
}

int main() {
    printf("Running unit tests for hello.c\n");
    printf("================================\n\n");
    
    TEST(get_greeting_returns_hello_world);
    TEST(get_greeting_not_null);
    TEST(get_greeting_not_empty);
    
    printf("\nTest Results:\n");
    printf("  Tests run: %d\n", tests_run);
    printf("  Tests passed: %d\n", tests_passed);
    printf("  Tests failed: %d\n", tests_run - tests_passed);
    
    if (tests_passed == tests_run) {
        printf("\nAll tests PASSED!\n");
        return 0;
    } else {
        printf("\nSome tests FAILED!\n");
        return 1;
    }
}