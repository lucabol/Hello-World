/* Plugin System Implementation
 * Manages plugin registration and execution
 */

#include "plugin.h"
#include <string.h>

/* Plugin registry - stores all registered plugins */
static const plugin_t* plugin_registry[MAX_PLUGINS];
static int plugin_registry_count = 0;

/* Working buffer for plugin transformations */
static char plugin_working_buffer[PLUGIN_BUFFER_SIZE];

/* Initialize the plugin system */
void plugin_init(void) {
    plugin_registry_count = 0;
    memset(plugin_working_buffer, 0, PLUGIN_BUFFER_SIZE);
}

/* Register a plugin */
int plugin_register(const plugin_t* plugin) {
    if (plugin_registry_count >= MAX_PLUGINS) {
        return -1;  /* Registry full */
    }
    
    plugin_registry[plugin_registry_count] = plugin;
    plugin_registry_count++;
    return 0;
}

/* Apply all registered plugins to input string */
const char* plugin_apply_all(const char* input) {
    const char* current = input;
    int i;
    
    /* If no plugins registered, return input unchanged */
    if (plugin_registry_count == 0) {
        return input;
    }
    
    /* Apply each plugin in sequence */
    for (i = 0; i < plugin_registry_count; i++) {
        if (plugin_registry[i] && plugin_registry[i]->transform) {
            current = plugin_registry[i]->transform(current);
        }
    }
    
    return current;
}

/* Get the number of registered plugins */
int plugin_count(void) {
    return plugin_registry_count;
}
