/* Plugin System Implementation */

#include "plugin.h"
#include <string.h>
#include <stdio.h>

/* Array of registered plugins */
static plugin_t plugins[MAX_PLUGINS];
static int plugin_count_value = 0;
static int initialized = 0;

/* Initialize plugin system */
void plugin_init(void) {
    if (!initialized) {
        plugin_count_value = 0;
        initialized = 1;
    }
}

/* Register a plugin */
int plugin_register(const char* name, const char* description, plugin_transform_func transform) {
    plugin_init();
    
    if (plugin_count_value >= MAX_PLUGINS) {
        return -1;
    }
    
    plugins[plugin_count_value].name = name;
    plugins[plugin_count_value].description = description;
    plugins[plugin_count_value].transform = transform;
    plugin_count_value++;
    
    return 0;
}

/* Apply all registered plugins to a message */
char* plugin_apply_all(const char* input, char* output, size_t output_size) {
    plugin_init();
    
    if (plugin_count_value == 0) {
        /* No plugins registered, return input as-is */
        strncpy(output, input, output_size - 1);
        output[output_size - 1] = '\0';
        return output;
    }
    
    /* Create temporary buffers for plugin chaining */
    char temp1[256];
    char temp2[256];
    const char* current_input = input;
    char* current_output = temp1;
    char* next_output = temp2;
    
    /* Apply each plugin in sequence */
    int i;
    for (i = 0; i < plugin_count_value; i++) {
        if (plugins[i].transform) {
            char* result = plugins[i].transform(current_input, current_output, 256);
            if (result) {
                current_input = result;
                /* Swap buffers */
                char* tmp = current_output;
                current_output = next_output;
                next_output = tmp;
            }
        }
    }
    
    /* Copy final result to output buffer */
    strncpy(output, current_input, output_size - 1);
    output[output_size - 1] = '\0';
    
    return output;
}

/* Get number of registered plugins */
int plugin_count(void) {
    plugin_init();
    return plugin_count_value;
}
