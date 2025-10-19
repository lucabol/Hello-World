/* Plugin System Implementation */

#include "plugin.h"
#include <stddef.h>

/* Plugin registry - global array of function pointers */
plugin_func_t plugin_registry[MAX_PLUGINS];
int plugin_count = 0;

/* Register a plugin function */
void register_plugin(plugin_func_t func) {
    if (plugin_count < MAX_PLUGINS && func != NULL) {
        plugin_registry[plugin_count++] = func;
    }
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
