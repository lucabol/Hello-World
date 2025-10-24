/* Plugin System Implementation
 * 
 * This file implements the plugin management system for hello.c.
 * It provides plugin registration, storage, and execution functionality.
 * 
 * Thread Safety: This implementation is NOT thread-safe. All operations
 * must be performed from a single thread or protected by external locking.
 */

#include "plugin.h"
#include <string.h>
#include <stdio.h>

/* Static plugin registry */
static plugin_t* plugin_registry[PLUGIN_MAX_COUNT];
static int plugin_count = 0;

/* Register a plugin with full validation */
int plugin_register(plugin_t* plugin) {
    /* Validate plugin pointer */
    if (plugin == NULL) {
        fprintf(stderr, "Error: Cannot register NULL plugin\n");
        return PLUGIN_ERR_NULL_POINTER;
    }
    
    /* Validate plugin fields */
    if (plugin->name == NULL) {
        fprintf(stderr, "Error: Plugin name cannot be NULL\n");
        return PLUGIN_ERR_INVALID_PLUGIN;
    }
    
    if (plugin->transform == NULL) {
        fprintf(stderr, "Error: Plugin transform function cannot be NULL\n");
        return PLUGIN_ERR_INVALID_PLUGIN;
    }
    
    /* Check registry capacity */
    if (plugin_count >= PLUGIN_MAX_COUNT) {
        fprintf(stderr, "Error: Plugin registry full (max %d plugins)\n", PLUGIN_MAX_COUNT);
        return PLUGIN_ERR_REGISTRY_FULL;
    }
    
    /* Register the plugin */
    plugin_registry[plugin_count] = plugin;
    plugin_count++;
    
    return PLUGIN_SUCCESS;
}

/* Unregister a plugin by name */
int plugin_unregister(const char* name) {
    int i;
    
    if (name == NULL) {
        fprintf(stderr, "Error: Cannot unregister plugin with NULL name\n");
        return PLUGIN_ERR_NULL_POINTER;
    }
    
    /* Find and remove the plugin */
    for (i = 0; i < plugin_count; i++) {
        if (plugin_registry[i] && plugin_registry[i]->name &&
            strcmp(plugin_registry[i]->name, name) == 0) {
            
            /* Shift remaining plugins down */
            int j;
            for (j = i; j < plugin_count - 1; j++) {
                plugin_registry[j] = plugin_registry[j + 1];
            }
            plugin_registry[plugin_count - 1] = NULL;
            plugin_count--;
            
            return PLUGIN_SUCCESS;
        }
    }
    
    return PLUGIN_ERR_PLUGIN_NOT_FOUND;
}

/* Apply all registered plugins to a message */
int plugin_apply_all(const char* input, char* output, size_t output_size) {
    int i;
    int result;
    
    /* Validate parameters */
    if (input == NULL) {
        fprintf(stderr, "Error: Input cannot be NULL\n");
        return PLUGIN_ERR_NULL_POINTER;
    }
    
    if (output == NULL) {
        fprintf(stderr, "Error: Output cannot be NULL\n");
        return PLUGIN_ERR_NULL_POINTER;
    }
    
    if (output_size == 0) {
        fprintf(stderr, "Error: Output buffer size cannot be 0\n");
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    /* If no plugins registered, just copy input to output */
    if (plugin_count == 0) {
        size_t input_len = strlen(input);
        if (input_len >= output_size) {
            fprintf(stderr, "Error: Input too large for output buffer\n");
            return PLUGIN_ERR_BUFFER_TOO_SMALL;
        }
        strncpy(output, input, output_size - 1);
        output[output_size - 1] = '\0';
        return 0;
    }
    
    /* Use two buffers for alternating transformations */
    char buffer1[PLUGIN_MAX_MESSAGE_LEN];
    char buffer2[PLUGIN_MAX_MESSAGE_LEN];
    char* current_input = buffer1;
    char* current_output = buffer2;
    
    /* Initialize first buffer with input */
    strncpy(current_input, input, PLUGIN_MAX_MESSAGE_LEN - 1);
    current_input[PLUGIN_MAX_MESSAGE_LEN - 1] = '\0';
    
    /* Apply each plugin in sequence */
    for (i = 0; i < plugin_count; i++) {
        plugin_t* plugin = plugin_registry[i];
        
        /* Validate plugin (defensive check) */
        if (plugin == NULL || plugin->transform == NULL) {
            fprintf(stderr, "Error: Invalid plugin at index %d\n", i);
            return PLUGIN_ERR_INVALID_PLUGIN;
        }
        
        /* Apply the plugin transformation */
        result = plugin->transform(current_input, current_output, PLUGIN_MAX_MESSAGE_LEN);
        
        if (result != PLUGIN_SUCCESS) {
            fprintf(stderr, "Error: Plugin '%s' failed with code %d\n", 
                    plugin->name ? plugin->name : "unknown", result);
            
            /* Copy the last successful result to output */
            strncpy(output, current_input, output_size - 1);
            output[output_size - 1] = '\0';
            
            return PLUGIN_ERR_TRANSFORM_FAILED;
        }
        
        /* Swap buffers for next iteration */
        char* temp = current_input;
        current_input = current_output;
        current_output = temp;
    }
    
    /* Copy final result to output (current_input has the last result) */
    size_t result_len = strlen(current_input);
    if (result_len >= output_size) {
        fprintf(stderr, "Error: Final result too large for output buffer\n");
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    strncpy(output, current_input, output_size - 1);
    output[output_size - 1] = '\0';
    
    return plugin_count;
}

/* Get the number of registered plugins */
int plugin_get_count(void) {
    return plugin_count;
}

/* Clear all registered plugins */
void plugin_clear(void) {
    int i;
    
    /* Clear the array */
    for (i = 0; i < PLUGIN_MAX_COUNT; i++) {
        plugin_registry[i] = NULL;
    }
    
    plugin_count = 0;
}
