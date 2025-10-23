/* Plugin System Implementation
 * Provides the runtime infrastructure for the plugin system
 */

#include "plugin.h"
#include <stdio.h>
#include <string.h>

/* Global plugin registry */
plugin_t plugin_registry[MAX_PLUGINS];
int plugin_count = 0;

/* Register a plugin */
int plugin_register(const char* name, 
                    plugin_transform_fn transform,
                    plugin_hook_fn before,
                    plugin_hook_fn after) {
    if (plugin_count >= MAX_PLUGINS) {
        fprintf(stderr, "ERROR: Maximum number of plugins (%d) reached. Cannot register '%s'\n", 
                MAX_PLUGINS, name);
        fprintf(stderr, "Increase MAX_PLUGINS at compile time with -DMAX_PLUGINS=N\n");
        return PLUGIN_ERROR_MAX_PLUGINS_EXCEEDED;
    }
    
    plugin_registry[plugin_count].name = name;
    plugin_registry[plugin_count].transform = transform;
    plugin_registry[plugin_count].before = before;
    plugin_registry[plugin_count].after = after;
    plugin_count++;
    
    return PLUGIN_SUCCESS;
}

/* Get the current plugin count */
int plugin_get_count(void) {
    return plugin_count;
}

/* Execute all transform functions in registered order 
 * Uses double-buffering to safely chain transforms
 */
int plugin_execute_transforms(const char* input, char* output, size_t output_size) {
    char buffer1[PLUGIN_BUFFER_SIZE];
    char buffer2[PLUGIN_BUFFER_SIZE];
    char* current_input;
    char* current_output;
    size_t input_len;
    int i;
    int result;
    
    /* Validate input */
    if (input == NULL || output == NULL || output_size == 0) {
        return PLUGIN_ERROR_INVALID_INPUT;
    }
    
    input_len = strlen(input);
    if (input_len >= PLUGIN_BUFFER_SIZE) {
        fprintf(stderr, "ERROR: Input message too long (%zu bytes, max %d)\n", 
                input_len, PLUGIN_BUFFER_SIZE - 1);
        return PLUGIN_ERROR_INVALID_INPUT;
    }
    
    /* Copy input to first buffer */
    strncpy(buffer1, input, PLUGIN_BUFFER_SIZE - 1);
    buffer1[PLUGIN_BUFFER_SIZE - 1] = '\0';
    
    current_input = buffer1;
    current_output = buffer2;
    
    /* Execute transforms, ping-ponging between buffers */
    for (i = 0; i < plugin_count; i++) {
        if (plugin_registry[i].transform != NULL) {
            result = plugin_registry[i].transform(current_input, current_output, PLUGIN_BUFFER_SIZE);
            if (result != PLUGIN_SUCCESS) {
                fprintf(stderr, "ERROR: Plugin '%s' transform failed (code %d)\n", 
                        plugin_registry[i].name, result);
                return result;
            }
            
            /* Swap buffers for next iteration */
            if (current_input == buffer1) {
                current_input = buffer2;
                current_output = buffer1;
            } else {
                current_input = buffer1;
                current_output = buffer2;
            }
        }
    }
    
    /* Copy final result to output */
    strncpy(output, current_input, output_size - 1);
    output[output_size - 1] = '\0';
    
    /* Check if output was truncated */
    if (strlen(current_input) >= output_size) {
        fprintf(stderr, "WARNING: Output truncated (%zu bytes, buffer %zu bytes)\n",
                strlen(current_input), output_size);
    }
    
    return PLUGIN_SUCCESS;
}

/* Execute all before hooks */
void plugin_execute_before_hooks(void) {
    int i;
    for (i = 0; i < plugin_count; i++) {
        if (plugin_registry[i].before != NULL) {
            plugin_registry[i].before();
        }
    }
}

/* Execute all after hooks */
void plugin_execute_after_hooks(void) {
    int i;
    for (i = 0; i < plugin_count; i++) {
        if (plugin_registry[i].after != NULL) {
            plugin_registry[i].after();
        }
    }
}
