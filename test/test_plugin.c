/* Unit tests for plugin system
 * Tests plugin registration, application, and chaining
 */
#include <stdio.h>
#include <string.h>
#include "../plugin.h"
#include "simple_test.h"

/* External plugin declarations */
extern const plugin_t uppercase_plugin;
extern const plugin_t reverse_plugin;
extern const plugin_t prefix_plugin;

/* Test plugin initialization */
void test_plugin_init(void) {
    plugin_init();
    TEST_ASSERT_EQUAL_INT(0, plugin_count());
}

/* Test plugin registration */
void test_plugin_register(void) {
    plugin_init();
    int result = plugin_register(&uppercase_plugin);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(1, plugin_count());
}

/* Test plugin registration limit */
void test_plugin_register_limit(void) {
    int i;
    int result;
    
    plugin_init();
    
    /* Register maximum number of plugins */
    for (i = 0; i < 10; i++) {
        result = plugin_register(&uppercase_plugin);
        tests_run++;
        if (result == 0) {
            tests_passed++;
        } else {
            tests_failed++;
            print_red("  ✗ ");
            printf("Plugin registration failed at index %d\n", i);
            return;
        }
    }
    
    /* Try to register one more - should fail */
    result = plugin_register(&uppercase_plugin);
    tests_run++;
    if (result == -1) {
        tests_passed++;
        print_green("  ✓ ");
        printf("Plugin registration limit test passed\n");
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("Plugin registration limit test failed - should return -1\n");
    }
}

/* Test no plugins applied */
void test_no_plugins(void) {
    const char* input = "Hello world!";
    const char* result;
    
    plugin_init();
    result = plugin_apply_all(input);
    
    TEST_ASSERT_EQUAL_STRING(input, result);
}

/* Test uppercase plugin */
void test_uppercase_plugin(void) {
    const char* input = "Hello world!";
    const char* expected = "HELLO WORLD!";
    const char* result;
    
    plugin_init();
    plugin_register(&uppercase_plugin);
    result = plugin_apply_all(input);
    
    TEST_ASSERT_EQUAL_STRING(expected, result);
}

/* Test reverse plugin */
void test_reverse_plugin(void) {
    const char* input = "Hello world!";
    const char* expected = "!dlrow olleH";
    const char* result;
    
    plugin_init();
    plugin_register(&reverse_plugin);
    result = plugin_apply_all(input);
    
    TEST_ASSERT_EQUAL_STRING(expected, result);
}

/* Test prefix plugin */
void test_prefix_plugin(void) {
    const char* input = "Hello world!";
    const char* expected = ">>> Hello world!";
    const char* result;
    
    plugin_init();
    plugin_register(&prefix_plugin);
    result = plugin_apply_all(input);
    
    TEST_ASSERT_EQUAL_STRING(expected, result);
}

/* Test plugin chaining */
void test_plugin_chaining(void) {
    const char* input = "Hello world!";
    const char* expected = ">>> HELLO WORLD!";
    const char* result;
    
    plugin_init();
    plugin_register(&prefix_plugin);
    plugin_register(&uppercase_plugin);
    result = plugin_apply_all(input);
    
    TEST_ASSERT_EQUAL_STRING(expected, result);
}

/* Test multiple plugin chain */
void test_multiple_plugin_chain(void) {
    const char* input = "Hello world!";
    const char* expected = "!DLROW OLLEH >>>";
    const char* result;
    
    plugin_init();
    plugin_register(&prefix_plugin);
    plugin_register(&uppercase_plugin);
    plugin_register(&reverse_plugin);
    result = plugin_apply_all(input);
    
    TEST_ASSERT_EQUAL_STRING(expected, result);
}

/* Main test runner */
int main(void) {
    printf("\n");
    print_yellow("========================================\n");
    printf("Running Unit Tests for Plugin System\n");
    print_yellow("========================================\n");
    printf("\n");
    
    RUN_TEST(test_plugin_init);
    RUN_TEST(test_plugin_register);
    RUN_TEST(test_plugin_register_limit);
    RUN_TEST(test_no_plugins);
    RUN_TEST(test_uppercase_plugin);
    RUN_TEST(test_reverse_plugin);
    RUN_TEST(test_prefix_plugin);
    RUN_TEST(test_plugin_chaining);
    RUN_TEST(test_multiple_plugin_chain);
    
    TEST_SUMMARY();
}
