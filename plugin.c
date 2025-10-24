/* Plugin System Implementation
 * 
 * This file implements the plugin management system for hello.c.
 * It provides plugin registration, storage, and execution functionality.
 */

#include "plugin.h"
#include <string.h>
#include <stdio.h>

/* Static plugin registry */
static plugin_t* plugin_registry[PLUGIN_MAX_COUNT];
static int plugin_count = 0;

/* Register a plugin */
int register_plugin(plugin_t* plugin) {
    if (plugin == NULL || plugin->transform == NULL) {
        return -1;
    }
    
    if (plugin_count >= PLUGIN_MAX_COUNT) {
        fprintf(stderr, "Error: Plugin registry full (max %d plugins)\n", PLUGIN_MAX_COUNT);
        return -1;
    }
    
    plugin_registry[plugin_count] = plugin;
    plugin_count++;
    
    return 0;
}

/* Apply all registered plugins to a message */
int apply_plugins(const char* input, char* output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) {
        return -1;
    }
    
    /* If no plugins registered, just copy input to output */
    if (plugin_count == 0) {
        size_t input_len = strlen(input);
        if (input_len >= output_size) {
            return -1;
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
    int i;
    for (i = 0; i < plugin_count; i++) {
        plugin_t* plugin = plugin_registry[i];
        
        if (plugin->transform(current_input, current_output, PLUGIN_MAX_MESSAGE_LEN) != 0) {
            fprintf(stderr, "Error: Plugin '%s' failed\n", plugin->name ? plugin->name : "unknown");
            return -1;
        }
        
        /* Swap buffers for next iteration */
        char* temp = current_input;
        current_input = current_output;
        current_output = temp;
    }
    
    /* Copy final result to output (current_input has the last result) */
    strncpy(output, current_input, output_size - 1);
    output[output_size - 1] = '\0';
    
    return plugin_count;
}

/* Get the number of registered plugins */
int get_plugin_count(void) {
    return plugin_count;
}

/* Clear all registered plugins */
void clear_plugins(void) {
    plugin_count = 0;
    /* Note: We don't free the plugin structures themselves as they may be static */
}
