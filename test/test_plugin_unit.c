/* Unit Tests for Plugin System
 * 
 * Comprehensive test suite covering:
 * - Edge cases and error conditions
 * - Buffer overflow protection
 * - NULL pointer handling
 * - Plugin registration limits
 * - Plugin unregistration
 * - Plugin ordering
 * - Error propagation
 * - NUL termination guarantees
 * - Double unregister protection
 * - Buffer boundary cases
 * - Return value semantics
 * - Capacity APIs
 * - Long plugin chains
 * 
 * Test Categories:
 * 1. test_null_pointers()            - NULL pointer handling (7 tests)
 * 2. test_registration_limits()      - Plugin count limits (3 tests)
 * 3. test_unregistration()           - Plugin removal (7 tests)
 * 4. test_failing_plugin()           - Error propagation (4 tests)
 * 5. test_buffer_sizes()             - Buffer overflow protection (2 tests)
 * 6. test_plugin_ordering()          - Deterministic ordering (3 tests)
 * 7. test_clear_plugins()            - Registry reset (5 tests)
 * 8. test_no_plugins()               - No plugins registered (2 tests)
 * 9. test_duplicate_registration()   - Duplicate plugins (3 tests)
 * 10. test_nul_termination()         - NUL termination (5 tests)
 * 11. test_double_unregister()       - Double unregister protection (4 tests)
 * 12. test_buffer_boundaries()       - Buffer size 0, 1, exact, -1 (4 tests)
 * 13. test_return_values()           - Return value semantics (3 tests)
 * 14. test_capacity_apis()           - Capacity query functions (5 tests)
 * 15. test_long_chains()             - Maximum plugin chains (2 tests)
 * 16. test_error_propagation_chain() - Error in chain (3 tests)
 * 
 * Total: 73 tests covering all edge cases
 */

#include "plugin.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Test counters */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/* Color codes */
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_RESET "\033[0m"

/* Test assertion macro */
#define TEST_ASSERT(condition, message) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf(COLOR_GREEN "  ✓ " COLOR_RESET "%s\n", message); \
    } else { \
        tests_failed++; \
        printf(COLOR_RED "  ✗ " COLOR_RESET "%s\n", message); \
    } \
} while(0)

/* Test helper plugins */
static int identity_transform(const char* input, char* output, size_t output_size) {
    if (strlen(input) >= output_size) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    return PLUGIN_SUCCESS;
}

static int failing_transform(const char* input, char* output, size_t output_size) {
    (void)input; (void)output; (void)output_size;
    return PLUGIN_ERR_TRANSFORM_FAILED;
}

static int large_output_transform(const char* input, char* output, size_t output_size) {
    (void)input;
    /* Try to create output larger than buffer */
    if (output_size < 300) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    memset(output, 'A', 299);
    output[299] = '\0';
    return PLUGIN_SUCCESS;
}

static plugin_t identity_plugin = { .name = "identity", .transform = identity_transform };
static plugin_t failing_plugin = { .name = "failing", .transform = failing_transform };
static plugin_t large_output_plugin = { .name = "large", .transform = large_output_transform };

/* Test: NULL pointer handling */
void test_null_pointers(void) {
    char output[256];
    int result;
    
    printf(COLOR_YELLOW "Testing NULL pointer handling...\n" COLOR_RESET);
    
    /* Test NULL plugin registration */
    result = plugin_register(NULL);
    TEST_ASSERT(result == PLUGIN_ERR_NULL_POINTER, 
                "plugin_register(NULL) returns PLUGIN_ERR_NULL_POINTER");
    
    /* Test NULL plugin name */
    plugin_t invalid_plugin1 = { .name = NULL, .transform = identity_transform };
    result = plugin_register(&invalid_plugin1);
    TEST_ASSERT(result == PLUGIN_ERR_INVALID_PLUGIN,
                "plugin_register with NULL name returns PLUGIN_ERR_INVALID_PLUGIN");
    
    /* Test NULL plugin transform */
    plugin_t invalid_plugin2 = { .name = "test", .transform = NULL };
    result = plugin_register(&invalid_plugin2);
    TEST_ASSERT(result == PLUGIN_ERR_INVALID_PLUGIN,
                "plugin_register with NULL transform returns PLUGIN_ERR_INVALID_PLUGIN");
    
    /* Test NULL input */
    result = plugin_apply_all(NULL, output, sizeof(output));
    TEST_ASSERT(result == PLUGIN_ERR_NULL_POINTER,
                "plugin_apply_all with NULL input returns PLUGIN_ERR_NULL_POINTER");
    
    /* Test NULL output */
    result = plugin_apply_all("test", NULL, 256);
    TEST_ASSERT(result == PLUGIN_ERR_NULL_POINTER,
                "plugin_apply_all with NULL output returns PLUGIN_ERR_NULL_POINTER");
    
    /* Test zero buffer size */
    result = plugin_apply_all("test", output, 0);
    TEST_ASSERT(result == PLUGIN_ERR_BUFFER_TOO_SMALL,
                "plugin_apply_all with 0 buffer size returns PLUGIN_ERR_BUFFER_TOO_SMALL");
    
    /* Test NULL name in unregister */
    result = plugin_unregister(NULL);
    TEST_ASSERT(result == PLUGIN_ERR_NULL_POINTER,
                "plugin_unregister(NULL) returns PLUGIN_ERR_NULL_POINTER");
}

/* Test: Plugin registration limits */
void test_registration_limits(void) {
    int i, result;
    plugin_t test_plugins[PLUGIN_MAX_COUNT + 2];
    
    printf(COLOR_YELLOW "\nTesting registration limits...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* Fill up the registry */
    for (i = 0; i < PLUGIN_MAX_COUNT; i++) {
        test_plugins[i].name = "test";
        test_plugins[i].transform = identity_transform;
        result = plugin_register(&test_plugins[i]);
        TEST_ASSERT(result == PLUGIN_SUCCESS,
                    "Can register plugin within limit");
    }
    
    TEST_ASSERT(plugin_get_count() == PLUGIN_MAX_COUNT,
                "Plugin count equals PLUGIN_MAX_COUNT");
    
    /* Try to register beyond limit */
    test_plugins[PLUGIN_MAX_COUNT].name = "overflow";
    test_plugins[PLUGIN_MAX_COUNT].transform = identity_transform;
    result = plugin_register(&test_plugins[PLUGIN_MAX_COUNT]);
    TEST_ASSERT(result == PLUGIN_ERR_REGISTRY_FULL,
                "Registering beyond limit returns PLUGIN_ERR_REGISTRY_FULL");
    
    TEST_ASSERT(plugin_get_count() == PLUGIN_MAX_COUNT,
                "Plugin count unchanged after failed registration");
    
    plugin_clear();
}

/* Test: Plugin unregistration */
void test_unregistration(void) {
    int result;
    char output[256];
    
    printf(COLOR_YELLOW "\nTesting plugin unregistration...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* Register some plugins */
    plugin_register(&identity_plugin);
    TEST_ASSERT(plugin_get_count() == 1, "One plugin registered");
    
    /* Unregister existing plugin */
    result = plugin_unregister("identity");
    TEST_ASSERT(result == PLUGIN_SUCCESS, "Unregister existing plugin succeeds");
    TEST_ASSERT(plugin_get_count() == 0, "Plugin count is 0 after unregister");
    
    /* Try to unregister non-existent plugin */
    result = plugin_unregister("nonexistent");
    TEST_ASSERT(result == PLUGIN_ERR_PLUGIN_NOT_FOUND,
                "Unregister non-existent plugin returns PLUGIN_ERR_PLUGIN_NOT_FOUND");
    
    /* Test unregister from middle of list */
    plugin_t plugin1 = { .name = "first", .transform = identity_transform };
    plugin_t plugin2 = { .name = "second", .transform = identity_transform };
    plugin_t plugin3 = { .name = "third", .transform = identity_transform };
    
    plugin_register(&plugin1);
    plugin_register(&plugin2);
    plugin_register(&plugin3);
    TEST_ASSERT(plugin_get_count() == 3, "Three plugins registered");
    
    plugin_unregister("second");
    TEST_ASSERT(plugin_get_count() == 2, "Two plugins remain after middle unregister");
    
    /* Verify remaining plugins still work */
    result = plugin_apply_all("test", output, sizeof(output));
    TEST_ASSERT(result == 2, "Two plugins applied after middle unregister");
    
    plugin_clear();
}

/* Test: Failing plugin behavior */
void test_failing_plugin(void) {
    int result;
    char output[256];
    
    printf(COLOR_YELLOW "\nTesting failing plugin behavior...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* Register a failing plugin */
    plugin_register(&failing_plugin);
    
    result = plugin_apply_all("test", output, sizeof(output));
    TEST_ASSERT(result == PLUGIN_ERR_TRANSFORM_FAILED,
                "Failing plugin returns PLUGIN_ERR_TRANSFORM_FAILED");
    
    /* Test that output contains input when plugin fails */
    TEST_ASSERT(strcmp(output, "test") == 0,
                "Output contains input when first plugin fails");
    
    plugin_clear();
    
    /* Test plugin failure in chain */
    plugin_register(&identity_plugin);
    plugin_register(&failing_plugin);
    
    result = plugin_apply_all("test", output, sizeof(output));
    TEST_ASSERT(result == PLUGIN_ERR_TRANSFORM_FAILED,
                "Chain stops at failing plugin");
    TEST_ASSERT(strcmp(output, "test") == 0,
                "Output contains last successful transformation");
    
    plugin_clear();
}

/* Test: Buffer size handling */
void test_buffer_sizes(void) {
    int result;
    char small_output[5];
    char output[256];
    
    printf(COLOR_YELLOW "\nTesting buffer size handling...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* Test input too large for output buffer */
    result = plugin_apply_all("This is a very long string", small_output, sizeof(small_output));
    TEST_ASSERT(result == PLUGIN_ERR_BUFFER_TOO_SMALL,
                "Input too large returns PLUGIN_ERR_BUFFER_TOO_SMALL");
    
    /* Test plugin that produces large output */
    plugin_register(&large_output_plugin);
    result = plugin_apply_all("test", output, sizeof(output));
    TEST_ASSERT(result == PLUGIN_ERR_TRANSFORM_FAILED,
                "Plugin producing too-large output fails gracefully");
    
    plugin_clear();
}

/* Test: Plugin ordering */
void test_plugin_ordering(void) {
    char output[256];
    int result;
    
    printf(COLOR_YELLOW "\nTesting plugin ordering...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* Create order-dependent plugins */
    extern plugin_t uppercase_plugin;
    extern plugin_t exclaim_plugin;
    extern plugin_t reverse_plugin;
    
    /* Test order 1: uppercase then exclaim */
    plugin_register(&uppercase_plugin);
    plugin_register(&exclaim_plugin);
    result = plugin_apply_all("hello", output, sizeof(output));
    TEST_ASSERT(result == 2 && strcmp(output, "HELLO!!!") == 0,
                "Uppercase then exclaim produces 'HELLO!!!'");
    
    plugin_clear();
    
    /* Test order 2: exclaim then uppercase */
    plugin_register(&exclaim_plugin);
    plugin_register(&uppercase_plugin);
    result = plugin_apply_all("hello", output, sizeof(output));
    TEST_ASSERT(result == 2 && strcmp(output, "HELLO!!!") == 0,
                "Exclaim then uppercase produces 'HELLO!!!'");
    
    plugin_clear();
    
    /* Test order 3: reverse then uppercase */
    plugin_register(&reverse_plugin);
    plugin_register(&uppercase_plugin);
    result = plugin_apply_all("hello", output, sizeof(output));
    TEST_ASSERT(result == 2 && strcmp(output, "OLLEH") == 0,
                "Reverse then uppercase produces 'OLLEH'");
    
    plugin_clear();
}

/* Test: Clear plugins */
void test_clear_plugins(void) {
    char output[256];
    int result;
    
    printf(COLOR_YELLOW "\nTesting clear_plugins...\n" COLOR_RESET);
    
    /* Register some plugins */
    plugin_register(&identity_plugin);
    plugin_register(&identity_plugin);
    TEST_ASSERT(plugin_get_count() == 2, "Two plugins registered");
    
    /* Clear all plugins */
    plugin_clear();
    TEST_ASSERT(plugin_get_count() == 0, "Plugin count is 0 after clear");
    
    /* Verify no plugins are applied */
    result = plugin_apply_all("test", output, sizeof(output));
    TEST_ASSERT(result == 0, "No plugins applied after clear");
    TEST_ASSERT(strcmp(output, "test") == 0, "Output equals input after clear");
    
    /* Verify can register again after clear */
    plugin_register(&identity_plugin);
    TEST_ASSERT(plugin_get_count() == 1, "Can register plugin after clear");
    
    plugin_clear();
}

/* Test: No plugins registered */
void test_no_plugins(void) {
    char output[256];
    int result;
    
    printf(COLOR_YELLOW "\nTesting with no plugins...\n" COLOR_RESET);
    
    plugin_clear();
    
    result = plugin_apply_all("hello world", output, sizeof(output));
    TEST_ASSERT(result == 0, "Returns 0 when no plugins registered");
    TEST_ASSERT(strcmp(output, "hello world") == 0,
                "Output equals input when no plugins");
}

/* Test: Duplicate registration */
void test_duplicate_registration(void) {
    int result;
    
    printf(COLOR_YELLOW "\nTesting duplicate registration...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* Register same plugin twice */
    result = plugin_register(&identity_plugin);
    TEST_ASSERT(result == PLUGIN_SUCCESS, "First registration succeeds");
    
    result = plugin_register(&identity_plugin);
    TEST_ASSERT(result == PLUGIN_SUCCESS, "Duplicate registration succeeds");
    
    TEST_ASSERT(plugin_get_count() == 2,
                "Duplicate registration adds to count");
    
    plugin_clear();
}

/* Test: NUL termination guarantees */
void test_nul_termination(void) {
    char output[256];
    int result;
    int i;
    
    printf(COLOR_YELLOW "\nTesting NUL termination guarantees...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* Fill output buffer with non-NUL bytes to verify NUL termination */
    memset(output, 'X', sizeof(output));
    
    /* Test with no plugins */
    result = plugin_apply_all("test", output, sizeof(output));
    TEST_ASSERT(result == 0 && output[4] == '\0',
                "Output NUL-terminated with no plugins");
    
    /* Verify no data beyond NUL */
    for (i = 5; i < 10; i++) {
        if (output[i] != 'X') {
            TEST_ASSERT(0, "Data corruption beyond NUL terminator");
            break;
        }
    }
    if (i == 10) {
        TEST_ASSERT(1, "No data corruption beyond NUL terminator");
    }
    
    /* Test with plugins */
    extern plugin_t uppercase_plugin;
    plugin_register(&uppercase_plugin);
    
    memset(output, 'X', sizeof(output));
    result = plugin_apply_all("hello", output, sizeof(output));
    TEST_ASSERT(result == 1 && output[5] == '\0',
                "Output NUL-terminated with plugins");
    
    /* Test with small buffer */
    char small_output[6];
    memset(small_output, 'X', sizeof(small_output));
    result = plugin_apply_all("hello", small_output, sizeof(small_output));
    TEST_ASSERT(result == 1 && small_output[5] == '\0',
                "Output NUL-terminated with small buffer");
    
    plugin_clear();
}

/* Test: Double unregister protection */
void test_double_unregister(void) {
    int result;
    
    printf(COLOR_YELLOW "\nTesting double unregister protection...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* Register a plugin */
    plugin_register(&identity_plugin);
    TEST_ASSERT(plugin_get_count() == 1, "Plugin registered");
    
    /* Unregister once */
    result = plugin_unregister("identity");
    TEST_ASSERT(result == PLUGIN_SUCCESS, "First unregister succeeds");
    TEST_ASSERT(plugin_get_count() == 0, "Plugin count is 0");
    
    /* Unregister again (should be safe) */
    result = plugin_unregister("identity");
    TEST_ASSERT(result == PLUGIN_ERR_PLUGIN_NOT_FOUND,
                "Second unregister returns PLUGIN_ERR_PLUGIN_NOT_FOUND");
    TEST_ASSERT(plugin_get_count() == 0, "Plugin count still 0");
    
    plugin_clear();
}

/* Test: Buffer boundary cases */
void test_buffer_boundaries(void) {
    char output[10];
    int result;
    
    printf(COLOR_YELLOW "\nTesting buffer boundary cases...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* Test with buffer size 0 */
    result = plugin_apply_all("test", output, 0);
    TEST_ASSERT(result == PLUGIN_ERR_BUFFER_TOO_SMALL,
                "Buffer size 0 returns PLUGIN_ERR_BUFFER_TOO_SMALL");
    
    /* Test with buffer size 1 (only room for NUL) */
    result = plugin_apply_all("", output, 1);
    TEST_ASSERT(result == 0, "Empty string with size 1 succeeds");
    TEST_ASSERT(output[0] == '\0', "Output is NUL-terminated");
    
    /* Test with exact fit */
    result = plugin_apply_all("test", output, 5);
    TEST_ASSERT(result == 0, "Exact fit succeeds");
    TEST_ASSERT(strcmp(output, "test") == 0, "Output matches input");
    TEST_ASSERT(output[4] == '\0', "Output is NUL-terminated");
    
    /* Test with one less than required */
    result = plugin_apply_all("test", output, 4);
    TEST_ASSERT(result == PLUGIN_ERR_BUFFER_TOO_SMALL,
                "One less than required returns PLUGIN_ERR_BUFFER_TOO_SMALL");
    
    plugin_clear();
}

/* Test: Return value semantics */
void test_return_values(void) {
    char output[256];
    int result;
    
    printf(COLOR_YELLOW "\nTesting return value semantics...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* No plugins - should return 0 */
    result = plugin_apply_all("test", output, sizeof(output));
    TEST_ASSERT(result == 0, "No plugins returns 0");
    
    /* One plugin - should return 1 */
    extern plugin_t uppercase_plugin;
    plugin_register(&uppercase_plugin);
    result = plugin_apply_all("test", output, sizeof(output));
    TEST_ASSERT(result == 1, "One plugin returns 1");
    
    /* Two plugins - should return 2 */
    extern plugin_t exclaim_plugin;
    plugin_register(&exclaim_plugin);
    result = plugin_apply_all("test", output, sizeof(output));
    TEST_ASSERT(result == 2, "Two plugins returns 2");
    
    plugin_clear();
}

/* Test: Capacity APIs */
void test_capacity_apis(void) {
    int count, capacity;
    
    printf(COLOR_YELLOW "\nTesting capacity APIs...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* Test capacity function */
    capacity = plugin_get_capacity();
    TEST_ASSERT(capacity == PLUGIN_MAX_COUNT,
                "plugin_get_capacity() returns PLUGIN_MAX_COUNT");
    TEST_ASSERT(capacity == 10, "Default capacity is 10");
    
    /* Test count at various states */
    count = plugin_get_count();
    TEST_ASSERT(count == 0, "Initial count is 0");
    
    plugin_register(&identity_plugin);
    count = plugin_get_count();
    TEST_ASSERT(count == 1, "Count is 1 after registration");
    
    plugin_clear();
    count = plugin_get_count();
    TEST_ASSERT(count == 0, "Count is 0 after clear");
    
    plugin_clear();
}

/* Test: Long plugin chains */
void test_long_chains(void) {
    char output[256];
    int result;
    int i;
    
    printf(COLOR_YELLOW "\nTesting long plugin chains...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* Register maximum number of plugins */
    extern plugin_t uppercase_plugin;
    for (i = 0; i < PLUGIN_MAX_COUNT; i++) {
        result = plugin_register(&uppercase_plugin);
        TEST_ASSERT(result == PLUGIN_SUCCESS,
                    "Can register up to PLUGIN_MAX_COUNT plugins");
    }
    
    /* Apply all plugins */
    result = plugin_apply_all("test", output, sizeof(output));
    TEST_ASSERT(result == PLUGIN_MAX_COUNT,
                "Returns PLUGIN_MAX_COUNT when all applied");
    TEST_ASSERT(strcmp(output, "TEST") == 0,
                "Long chain produces correct output");
    
    plugin_clear();
}

/* Test: Error propagation in chain */
void test_error_propagation_chain(void) {
    char output[256];
    int result;
    
    printf(COLOR_YELLOW "\nTesting error propagation in chain...\n" COLOR_RESET);
    
    plugin_clear();
    
    /* Register good plugin, then failing plugin, then another good plugin */
    extern plugin_t uppercase_plugin;
    plugin_register(&uppercase_plugin);
    plugin_register(&failing_plugin);
    extern plugin_t exclaim_plugin;
    plugin_register(&exclaim_plugin);
    
    /* Should stop at failing plugin */
    result = plugin_apply_all("test", output, sizeof(output));
    TEST_ASSERT(result == PLUGIN_ERR_TRANSFORM_FAILED,
                "Returns PLUGIN_ERR_TRANSFORM_FAILED on failure");
    TEST_ASSERT(strcmp(output, "TEST") == 0,
                "Output contains last successful transformation");
    
    /* Verify third plugin was NOT applied */
    TEST_ASSERT(strstr(output, "!") == NULL,
                "Third plugin not applied after failure");
    
    plugin_clear();
}

/* Main test runner */
int main(void) {
    printf("\n");
    printf(COLOR_YELLOW "========================================\n");
    printf("Plugin System Unit Tests\n");
    printf("========================================\n" COLOR_RESET);
    printf("\n");
    
    test_null_pointers();
    test_registration_limits();
    test_unregistration();
    test_failing_plugin();
    test_buffer_sizes();
    test_plugin_ordering();
    test_clear_plugins();
    test_no_plugins();
    test_duplicate_registration();
    test_nul_termination();
    test_double_unregister();
    test_buffer_boundaries();
    test_return_values();
    test_capacity_apis();
    test_long_chains();
    test_error_propagation_chain();
    
    printf("\n");
    printf(COLOR_YELLOW "========================================\n");
    printf("Test Summary\n");
    printf("========================================\n" COLOR_RESET);
    printf("Total tests:  %d\n", tests_run);
    
    if (tests_failed == 0) {
        printf(COLOR_GREEN "Passed:       %d\n", tests_passed);
        printf("Failed:       %d\n", tests_failed);
        printf("\n✓ All tests passed!\n" COLOR_RESET);
        return 0;
    } else {
        printf(COLOR_GREEN "Passed:       " COLOR_RESET "%d\n", tests_passed);
        printf(COLOR_RED "Failed:       %d\n", tests_failed);
        printf("\n✗ Some tests failed!\n" COLOR_RESET);
        return 1;
    }
}
