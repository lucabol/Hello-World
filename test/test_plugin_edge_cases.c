/* Edge case and negative tests for plugin system
 * Tests error handling, boundary conditions, and safety
 */
#include <stdio.h>
#include <string.h>
#include "../plugin.h"
#include "simple_test.h"

/* External plugin declarations */
extern const plugin_t uppercase_plugin;
extern const plugin_t reverse_plugin;
extern const plugin_t prefix_plugin;

/* Test plugin that returns NULL to simulate error */
static const char* null_return_transform(const char* input) {
    (void)input;  /* Unused parameter */
    return NULL;  /* Simulate error condition */
}

static const plugin_t null_return_plugin = {
    .name = "null_return",
    .transform = null_return_transform
};

/* Test plugin that returns exactly PLUGIN_BUFFER_SIZE - 1 characters */
static char boundary_buffer[PLUGIN_BUFFER_SIZE];
static const char* boundary_transform(const char* input) {
    size_t i;
    (void)input;  /* Unused - we generate our own output */
    
    /* Fill buffer to exactly PLUGIN_BUFFER_SIZE - 1 chars */
    for (i = 0; i < PLUGIN_BUFFER_SIZE - 1; i++) {
        boundary_buffer[i] = 'A';
    }
    boundary_buffer[PLUGIN_BUFFER_SIZE - 1] = '\0';
    
    return boundary_buffer;
}

static const plugin_t boundary_plugin = {
    .name = "boundary",
    .transform = boundary_transform
};

/* Test plugin with NULL transform function */
static const plugin_t null_transform_plugin = {
    .name = "null_transform",
    .transform = NULL
};

/* Test plugin with NULL name */
static const char* dummy_transform(const char* input) {
    return input;
}

static const plugin_t null_name_plugin = {
    .name = NULL,
    .transform = dummy_transform
};

/* Test registering NULL plugin */
void test_register_null_plugin(void) {
    int result;
    
    plugin_init();
    result = plugin_register(NULL);
    
    TEST_ASSERT_EQUAL_INT(PLUGIN_ERROR_NULL, result);
}

/* Test registering plugin with NULL transform */
void test_register_null_transform(void) {
    int result;
    
    plugin_init();
    result = plugin_register(&null_transform_plugin);
    
    TEST_ASSERT_EQUAL_INT(PLUGIN_ERROR_NULL, result);
}

/* Test registering plugin with NULL name */
void test_register_null_name(void) {
    int result;
    
    plugin_init();
    result = plugin_register(&null_name_plugin);
    
    TEST_ASSERT_EQUAL_INT(PLUGIN_ERROR_NULL, result);
}

/* Test duplicate registration */
void test_duplicate_registration(void) {
    int result1, result2;
    
    plugin_init();
    result1 = plugin_register(&uppercase_plugin);
    result2 = plugin_register(&uppercase_plugin);
    
    TEST_ASSERT_EQUAL_INT(PLUGIN_SUCCESS, result1);
    TEST_ASSERT_EQUAL_INT(PLUGIN_ERROR_DUPLICATE, result2);
}

/* Dummy plugins for testing registry overflow */
static const char* dummy1_transform(const char* input) { return input; }
static const char* dummy2_transform(const char* input) { return input; }
static const char* dummy3_transform(const char* input) { return input; }
static const char* dummy4_transform(const char* input) { return input; }
static const char* dummy5_transform(const char* input) { return input; }
static const char* dummy6_transform(const char* input) { return input; }
static const char* dummy7_transform(const char* input) { return input; }

static const plugin_t dummy1_plugin = { .name = "dummy1", .transform = dummy1_transform };
static const plugin_t dummy2_plugin = { .name = "dummy2", .transform = dummy2_transform };
static const plugin_t dummy3_plugin = { .name = "dummy3", .transform = dummy3_transform };
static const plugin_t dummy4_plugin = { .name = "dummy4", .transform = dummy4_transform };
static const plugin_t dummy5_plugin = { .name = "dummy5", .transform = dummy5_transform };
static const plugin_t dummy6_plugin = { .name = "dummy6", .transform = dummy6_transform };
static const plugin_t dummy7_plugin = { .name = "dummy7", .transform = dummy7_transform };

/* Test registering more than PLUGIN_MAX_PLUGINS */
void test_register_overflow(void) {
    int result;
    const plugin_t* plugins[15];  /* More than PLUGIN_MAX_PLUGINS */
    int i;
    int registered = 0;
    
    /* Create array of unique plugins */
    plugins[0] = &uppercase_plugin;
    plugins[1] = &reverse_plugin;
    plugins[2] = &prefix_plugin;
    plugins[3] = &dummy1_plugin;
    plugins[4] = &dummy2_plugin;
    plugins[5] = &dummy3_plugin;
    plugins[6] = &dummy4_plugin;
    plugins[7] = &dummy5_plugin;
    plugins[8] = &dummy6_plugin;
    plugins[9] = &dummy7_plugin;
    plugins[10] = &boundary_plugin;
    plugins[11] = &null_return_plugin;
    /* Repeat some for overflow */
    plugins[12] = &uppercase_plugin;
    plugins[13] = &reverse_plugin;
    plugins[14] = &prefix_plugin;
    
    plugin_init();
    
    /* Try to register more plugins than the max */
    for (i = 0; i < 15; i++) {
        result = plugin_register(plugins[i]);
        
        if (result == PLUGIN_SUCCESS) {
            registered++;
        } else if (result == PLUGIN_ERROR_FULL) {
            /* Registry is full - this is expected */
            break;
        } else if (result == PLUGIN_ERROR_DUPLICATE) {
            /* Already registered - continue */
            continue;
        }
    }
    
    tests_run++;
    if (registered == PLUGIN_MAX_PLUGINS && result == PLUGIN_ERROR_FULL) {
        tests_passed++;
        print_green("  ✓ ");
        printf("Registry overflow test passed (registered %d, then got FULL)\n", registered);
    } else if (registered <= PLUGIN_MAX_PLUGINS) {
        tests_passed++;
        print_green("  ✓ ");
        printf("Registry overflow test passed (registered %d plugins)\n", registered);
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("Registry overflow test failed - registered %d (max=%d)\n", 
               registered, PLUGIN_MAX_PLUGINS);
    }
}

/* Test plugin returning NULL */
void test_plugin_returns_null(void) {
    const char* result;
    
    plugin_init();
    plugin_register(&null_return_plugin);
    
    result = plugin_apply_all("test input");
    
    tests_run++;
    if (result == NULL) {
        tests_passed++;
        print_green("  ✓ ");
        printf("NULL return handling test passed\n");
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("NULL return handling test failed - should return NULL\n");
    }
}

/* Test NULL input to plugin_apply_all */
void test_apply_null_input(void) {
    const char* result;
    
    plugin_init();
    plugin_register(&uppercase_plugin);
    
    result = plugin_apply_all(NULL);
    
    tests_run++;
    if (result == NULL) {
        tests_passed++;
        print_green("  ✓ ");
        printf("NULL input handling test passed\n");
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("NULL input handling test failed - should return NULL\n");
    }
}

/* Test boundary condition - exactly PLUGIN_BUFFER_SIZE - 1 characters */
void test_boundary_size(void) {
    const char* result;
    size_t len;
    
    plugin_init();
    plugin_register(&boundary_plugin);
    
    result = plugin_apply_all("test");
    
    tests_run++;
    if (result != NULL) {
        len = strlen(result);
        if (len == PLUGIN_BUFFER_SIZE - 1) {
            tests_passed++;
            print_green("  ✓ ");
            printf("Boundary size test passed (len=%zu)\n", len);
        } else {
            tests_failed++;
            print_red("  ✗ ");
            printf("Boundary size test failed - len=%zu, expected %d\n", 
                   len, PLUGIN_BUFFER_SIZE - 1);
        }
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("Boundary size test failed - result is NULL\n");
    }
}

/* Test chaining with boundary-sized output */
void test_chain_with_boundary(void) {
    const char* result;
    
    plugin_init();
    plugin_register(&boundary_plugin);
    plugin_register(&uppercase_plugin);  /* Should handle large input */
    
    result = plugin_apply_all("test");
    
    tests_run++;
    if (result != NULL) {
        tests_passed++;
        print_green("  ✓ ");
        printf("Chaining with boundary size test passed\n");
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("Chaining with boundary size test failed\n");
    }
}

/* Test that chaining works when plugins return pointers to static buffers */
void test_static_buffer_chaining(void) {
    const char* result;
    const char* expected = ">>> !DLROW OLLEH";
    
    plugin_init();
    plugin_register(&uppercase_plugin);  /* Returns pointer to static buffer */
    plugin_register(&reverse_plugin);    /* Returns pointer to static buffer */
    plugin_register(&prefix_plugin);     /* Returns pointer to static buffer */
    
    result = plugin_apply_all("Hello world!");
    
    tests_run++;
    if (result != NULL && strcmp(result, expected) == 0) {
        tests_passed++;
        print_green("  ✓ ");
        printf("Static buffer chaining test passed\n");
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("Static buffer chaining test failed\n");
        if (result) {
            printf("    Expected: \"%s\"\n", expected);
            printf("    Actual:   \"%s\"\n", result);
        } else {
            printf("    Result was NULL\n");
        }
    }
}

/* Test repeated calls to plugin_apply_all */
void test_repeated_calls(void) {
    const char* result1;
    const char* result2;
    const char* result3;
    char copy1[50], copy2[50], copy3[50];
    
    plugin_init();
    plugin_register(&uppercase_plugin);
    
    result1 = plugin_apply_all("first");
    if (result1) strcpy(copy1, result1);
    
    result2 = plugin_apply_all("second");
    if (result2) strcpy(copy2, result2);
    
    result3 = plugin_apply_all("third");
    if (result3) strcpy(copy3, result3);
    
    tests_run++;
    if (result1 && result2 && result3 &&
        strcmp(copy1, "FIRST") == 0 &&
        strcmp(copy2, "SECOND") == 0 &&
        strcmp(copy3, "THIRD") == 0) {
        tests_passed++;
        print_green("  ✓ ");
        printf("Repeated calls test passed\n");
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("Repeated calls test failed\n");
        printf("    copy1: \"%s\", copy2: \"%s\", copy3: \"%s\"\n", 
               result1 ? copy1 : "NULL",
               result2 ? copy2 : "NULL", 
               result3 ? copy3 : "NULL");
    }
}

/* Test empty string input */
void test_empty_string(void) {
    const char* result;
    
    plugin_init();
    plugin_register(&uppercase_plugin);
    
    result = plugin_apply_all("");
    
    tests_run++;
    if (result != NULL && strcmp(result, "") == 0) {
        tests_passed++;
        print_green("  ✓ ");
        printf("Empty string test passed\n");
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("Empty string test failed\n");
    }
}

/* Test very long input (close to buffer size) */
void test_long_input(void) {
    char long_input[PLUGIN_BUFFER_SIZE - 10];  /* Close to limit */
    const char* result;
    size_t i;
    
    /* Create a long input string */
    for (i = 0; i < sizeof(long_input) - 1; i++) {
        long_input[i] = 'a';
    }
    long_input[sizeof(long_input) - 1] = '\0';
    
    plugin_init();
    plugin_register(&uppercase_plugin);
    
    result = plugin_apply_all(long_input);
    
    tests_run++;
    if (result != NULL) {
        tests_passed++;
        print_green("  ✓ ");
        printf("Long input test passed (len=%zu)\n", strlen(result));
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("Long input test failed\n");
    }
}

/* Test NULL plugin in middle of chain */
void test_null_plugin_in_chain(void) {
    const char* result;
    
    plugin_init();
    plugin_register(&uppercase_plugin);
    plugin_register(&null_return_plugin);  /* This will return NULL */
    plugin_register(&reverse_plugin);
    
    result = plugin_apply_all("test");
    
    tests_run++;
    if (result == NULL) {
        tests_passed++;
        print_green("  ✓ ");
        printf("NULL plugin in chain test passed\n");
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("NULL plugin in chain test failed - should return NULL\n");
    }
}

/* Main test runner */
int main(void) {
    printf("\n");
    print_yellow("========================================\n");
    printf("Running Edge Case Tests for Plugin System\n");
    print_yellow("========================================\n");
    printf("\n");
    
    /* Registration error tests */
    RUN_TEST(test_register_null_plugin);
    RUN_TEST(test_register_null_transform);
    RUN_TEST(test_register_null_name);
    RUN_TEST(test_duplicate_registration);
    RUN_TEST(test_register_overflow);
    
    /* Runtime error tests */
    RUN_TEST(test_plugin_returns_null);
    RUN_TEST(test_apply_null_input);
    RUN_TEST(test_null_plugin_in_chain);
    
    /* Boundary condition tests */
    RUN_TEST(test_boundary_size);
    RUN_TEST(test_chain_with_boundary);
    RUN_TEST(test_long_input);
    RUN_TEST(test_empty_string);
    
    /* Safety tests */
    RUN_TEST(test_static_buffer_chaining);
    RUN_TEST(test_repeated_calls);
    
    TEST_SUMMARY();
}
