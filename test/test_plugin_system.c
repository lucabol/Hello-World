/* Unit tests for plugin system
 * Tests deduplication, overflow handling, and edge cases
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../plugin.h"
#include "../hello.h"

/* Reset plugin count for testing (extern variables) */
extern plugin_func_t plugin_registry[MAX_PLUGINS];
extern int plugin_count;

void reset_plugins(void) {
    int i;
    plugin_count = 0;
    for (i = 0; i < MAX_PLUGINS; i++) {
        plugin_registry[i] = NULL;
    }
}

/* Mock plugin functions for testing */
static char test_buffer1[256];
const char* test_plugin1(const char* msg) {
    snprintf(test_buffer1, sizeof(test_buffer1), "[1:%s]", msg);
    return test_buffer1;
}

static char test_buffer2[256];
const char* test_plugin2(const char* msg) {
    snprintf(test_buffer2, sizeof(test_buffer2), "[2:%s]", msg);
    return test_buffer2;
}

/* Test: Basic registration */
void test_basic_registration(void) {
    reset_plugins();
    int result = register_plugin(test_plugin1);
    assert(result == 0);
    assert(plugin_count == 1);
    printf("✓ test_basic_registration passed\n");
}

/* Test: Duplicate registration should be detected */
void test_duplicate_registration(void) {
    reset_plugins();
    int result1 = register_plugin(test_plugin1);
    int result2 = register_plugin(test_plugin1);
    assert(result1 == 0);
    assert(result2 == -2);  /* -2 indicates duplicate */
    assert(plugin_count == 1);  /* Should still be 1 */
    printf("✓ test_duplicate_registration passed\n");
}

/* Test: NULL plugin registration */
void test_null_registration(void) {
    reset_plugins();
    int result = register_plugin(NULL);
    assert(result == -1);
    assert(plugin_count == 0);
    printf("✓ test_null_registration passed\n");
}

/* Test: Registry overflow */
void test_registry_overflow(void) {
    reset_plugins();
    int i, result;
    
    /* Fill the registry */
    for (i = 0; i < MAX_PLUGINS; i++) {
        /* Use different addresses by adding offset */
        plugin_func_t dummy = (plugin_func_t)((char*)test_plugin1 + i);
        result = register_plugin(dummy);
        assert(result == 0);
    }
    assert(plugin_count == MAX_PLUGINS);
    
    /* Try to add one more - should fail */
    result = register_plugin(test_plugin2);
    assert(result == -1);  /* -1 indicates full */
    assert(plugin_count == MAX_PLUGINS);
    printf("✓ test_registry_overflow passed\n");
}

/* Test: Plugin chain application */
void test_plugin_chain(void) {
    reset_plugins();
    register_plugin(test_plugin1);
    register_plugin(test_plugin2);
    
    const char* result = apply_plugins("test");
    /* Should be [2:[1:test]] */
    assert(strcmp(result, "[2:[1:test]]") == 0);
    printf("✓ test_plugin_chain passed\n");
}

/* Test: Empty plugin chain (no plugins) */
void test_empty_chain(void) {
    reset_plugins();
    const char* input = "test";
    const char* result = apply_plugins(input);
    assert(result == input);  /* Should return input unchanged */
    printf("✓ test_empty_chain passed\n");
}

/* Test: Long input handling */
void test_long_input(void) {
    reset_plugins();
    /* Create a long string */
    char long_input[512];
    int i;
    for (i = 0; i < 500; i++) {
        long_input[i] = 'A';
    }
    long_input[500] = '\0';
    
    register_plugin(test_plugin1);
    const char* result = apply_plugins(long_input);
    
    /* Result should be truncated but valid */
    assert(result != NULL);
    assert(strlen(result) < 256);  /* Should fit in buffer */
    printf("✓ test_long_input passed\n");
}

int main(void) {
    printf("\n========================================\n");
    printf("Running Plugin System Tests\n");
    printf("========================================\n\n");
    
    test_basic_registration();
    test_duplicate_registration();
    test_null_registration();
    test_registry_overflow();
    test_plugin_chain();
    test_empty_chain();
    test_long_input();
    
    printf("\n========================================\n");
    printf("✓ All Plugin System Tests Passed!\n");
    printf("========================================\n\n");
    
    return 0;
}
