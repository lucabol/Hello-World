/* Test Plugin: NULL Return Handler
 * 
 * This plugin demonstrates error handling by returning NULL.
 * When a plugin returns NULL, the transformation is skipped and
 * the message is passed unchanged to the next plugin.
 * 
 * This is useful for:
 * - Conditional transformations (only transform if conditions are met)
 * - Error handling (return NULL on failure)
 * - Validation (skip transformation if input is invalid)
 */

#include "../plugin.h"
#include <stdio.h>
#include <string.h>

/* This plugin only transforms messages containing "test"
 * All other messages are passed through unchanged (by returning NULL)
 */
static char* conditional_transform(const char* input) {
    static char buffer[256];
    
    /* Only transform if input contains "test" */
    if (input && strstr(input, "test") != NULL) {
        snprintf(buffer, sizeof(buffer), "[TEST] %s", input);
        return buffer;
    }
    
    /* Return NULL to skip transformation for non-test messages */
    return NULL;
}

/* Define the plugin */
static plugin_interface null_test_plugin = {
    .process_message = conditional_transform,
    .plugin_name = "NullTestPlugin",
    .plugin_version = "1.0.0"
};

/* Register the plugin */
REGISTER_PLUGIN(null_test_plugin);
