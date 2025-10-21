/* Plugin System Implementation */

#include "plugin.h"
#include <stddef.h>
#include <stdio.h>

/* Plugin registry - global array of function pointers */
plugin_func_t plugin_registry[MAX_PLUGINS];
int plugin_count = 0;

/* Register a plugin function with deduplication and overflow handling
 * Returns: 0 on success, -1 if registry is full, -2 if already registered
 */
int register_plugin(plugin_func_t func) {
    int i;
    
    /* Check for NULL function pointer */
    if (func == NULL) {
        return -1;
    }
    
    /* Check for duplicate registration */
    for (i = 0; i < plugin_count; i++) {
        if (plugin_registry[i] == func) {
            /* Already registered - return success but indicate duplicate */
            return -2;
        }
    }
    
    /* Check if registry is full */
    if (plugin_count >= MAX_PLUGINS) {
        fprintf(stderr, "Warning: Plugin registry full (max %d plugins). Cannot register plugin.\n",
                MAX_PLUGINS);
        return -1;
    }
    
    /* Register the plugin */
    plugin_registry[plugin_count++] = func;
    return 0;
}

/* Apply all registered plugins to a message */
const char* apply_plugins(const char* message) {
    const char* result = message;
    int i;
    for (i = 0; i < plugin_count; i++) {
        if (plugin_registry[i] != NULL) {
            result = plugin_registry[i](result);
        }
    }
    return result;
}
