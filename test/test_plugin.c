/* Plugin System Tests
 * 
 * Unit tests for the plugin system functionality
 */
#include <stdio.h>
#include <string.h>
#include "../test/simple_test.h"
#include "plugin.h"

/* Mock plugin for testing */
static int mock_init_called = 0;
static int mock_transform_called = 0;
static int mock_cleanup_called = 0;

static int mock_init(void) {
    mock_init_called = 1;
    return PLUGIN_SUCCESS;
}

static int mock_transform(const char* input, char* output, size_t output_size) {
    if (!input || !output || output_size == 0) {
        return PLUGIN_ERROR;
    }
    mock_transform_called = 1;
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    return PLUGIN_SUCCESS;
}

static int mock_cleanup(void) {
    mock_cleanup_called = 1;
    return PLUGIN_SUCCESS;
}

static plugin_info_t mock_plugin = {
    .api_version = PLUGIN_API_VERSION,
    .name = "mock",
    .version = "1.0.0",
    .init = mock_init,
    .transform = mock_transform,
    .cleanup = mock_cleanup
};

/* Test: Plugin system initialization */
void test_plugin_system_init(void) {
    plugin_system_init();
    /* If we get here without crashing, init succeeded */
    tests_run++;
    tests_passed++;
    print_green("  ✓ ");
    printf("Plugin system initialization test passed\n");
}

/* Test: Plugin registration */
void test_plugin_register(void) {
    plugin_system_init();
    int result = plugin_register(&mock_plugin);
    TEST_ASSERT_EQUAL_INT(PLUGIN_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(1, mock_init_called);
    plugin_system_cleanup();
}

/* Test: Plugin transform */
void test_plugin_transform(void) {
    plugin_system_init();
    plugin_register(&mock_plugin);
    
    char output[256];
    int result = plugin_apply_all("Hello world!", output, sizeof(output));
    
    TEST_ASSERT_EQUAL_INT(PLUGIN_SUCCESS, result);
    TEST_ASSERT_EQUAL_STRING("Hello world!", output);
    TEST_ASSERT_EQUAL_INT(1, mock_transform_called);
    
    plugin_system_cleanup();
}

/* Test: Plugin cleanup */
void test_plugin_cleanup(void) {
    plugin_system_init();
    plugin_register(&mock_plugin);
    plugin_system_cleanup();
    
    TEST_ASSERT_EQUAL_INT(1, mock_cleanup_called);
}

/* Test: Apply with no plugins */
void test_apply_no_plugins(void) {
    plugin_system_init();
    
    char output[256];
    int result = plugin_apply_all("Hello world!", output, sizeof(output));
    
    TEST_ASSERT_EQUAL_INT(PLUGIN_SUCCESS, result);
    TEST_ASSERT_EQUAL_STRING("Hello world!", output);
    
    plugin_system_cleanup();
}

/* Test: Invalid plugin (NULL transform) */
void test_invalid_plugin_null_transform(void) {
    plugin_system_init();
    
    plugin_info_t invalid_plugin = {
        .api_version = PLUGIN_API_VERSION,
        .name = "invalid",
        .version = "1.0.0",
        .init = NULL,
        .transform = NULL,  /* Invalid: missing transform */
        .cleanup = NULL
    };
    
    int result = plugin_register(&invalid_plugin);
    TEST_ASSERT_EQUAL_INT(PLUGIN_ERROR, result);
    
    plugin_system_cleanup();
}

/* Test: Invalid API version */
void test_invalid_api_version(void) {
    plugin_system_init();
    
    plugin_info_t invalid_plugin = {
        .api_version = 999,  /* Invalid version */
        .name = "invalid",
        .version = "1.0.0",
        .init = NULL,
        .transform = mock_transform,
        .cleanup = NULL
    };
    
    int result = plugin_register(&invalid_plugin);
    TEST_ASSERT_EQUAL_INT(PLUGIN_ERROR, result);
    
    plugin_system_cleanup();
}

/* Main test runner */
int main(void) {
    printf("\n");
    print_yellow("========================================\n");
    printf("Running Plugin System Unit Tests\n");
    print_yellow("========================================\n");
    printf("\n");
    
    /* Reset mock counters */
    mock_init_called = 0;
    mock_transform_called = 0;
    mock_cleanup_called = 0;
    
    RUN_TEST(test_plugin_system_init);
    
    mock_init_called = 0;
    RUN_TEST(test_plugin_register);
    
    mock_init_called = 0;
    mock_transform_called = 0;
    RUN_TEST(test_plugin_transform);
    
    mock_init_called = 0;
    mock_transform_called = 0;
    mock_cleanup_called = 0;
    RUN_TEST(test_plugin_cleanup);
    
    RUN_TEST(test_apply_no_plugins);
    RUN_TEST(test_invalid_plugin_null_transform);
    RUN_TEST(test_invalid_api_version);
    
    TEST_SUMMARY();
}
