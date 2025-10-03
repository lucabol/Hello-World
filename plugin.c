/* Plugin System Implementation for Hello World
 * 
 * This file implements the plugin loading and processing mechanism.
 * Plugins are registered at compile time using constructor attributes.
 */

#include "plugin.h"
#include <stdio.h>
#include <string.h>

/* Plugin registry storage */
plugin_interface* plugin_registry[MAX_PLUGINS];
int plugin_count = 0;

/* Register a plugin in the registry */
void register_plugin(plugin_interface* plugin) {
    if (plugin_count >= MAX_PLUGINS) {
        fprintf(stderr, "Warning: Maximum number of plugins (%d) reached. Cannot register '%s'\n",
                MAX_PLUGINS, plugin->plugin_name);
        return;
    }
    
    plugin_registry[plugin_count] = plugin;
    plugin_count++;
    
    /* Optional: Print debug info (can be disabled with -DSILENT_PLUGINS) */
#ifndef SILENT_PLUGINS
    fprintf(stderr, "Plugin registered: %s (version %s)\n", 
            plugin->plugin_name, plugin->plugin_version);
#endif
}

/* Apply all registered plugins to a message */
const char* apply_plugins(const char* message) {
    const char* result = message;
    int i;
    
    /* Apply each plugin in registration order */
    for (i = 0; i < plugin_count; i++) {
        if (plugin_registry[i] && plugin_registry[i]->process_message) {
            char* transformed = plugin_registry[i]->process_message(result);
            if (transformed != NULL) {
                result = transformed;
            }
        }
    }
    
    return result;
}
