/* Plugin System Implementation for Hello World
 * 
 * This file implements the plugin loading and processing mechanism.
 * Plugins are registered at compile time using constructor attributes.
 * 
 * MEMORY MANAGEMENT:
 * - Plugin registry stores pointers to plugin_interface structures
 * - Plugins must ensure their plugin_interface structures remain valid
 * - No dynamic memory allocation is used by the plugin system itself
 * 
 * ERROR HANDLING:
 * - If a plugin returns NULL, the transformation is skipped
 * - The previous result is passed to the next plugin in the chain
 * - Warnings are printed to stderr for debugging
 */

#include "plugin.h"
#include <stdio.h>
#include <string.h>

/* Plugin registry storage */
plugin_interface* plugin_registry[MAX_PLUGINS];
int plugin_count = 0;

/* Register a plugin in the registry
 * 
 * This function is called automatically by REGISTER_PLUGIN macro during
 * program initialization (before main). It can also be called manually
 * if needed.
 * 
 * @param plugin - Pointer to plugin_interface (must remain valid for program lifetime)
 */
void register_plugin(plugin_interface* plugin) {
    /* Validate input */
    if (plugin == NULL) {
        fprintf(stderr, "Error: Attempted to register NULL plugin\n");
        return;
    }
    
    if (plugin->plugin_name == NULL) {
        fprintf(stderr, "Error: Plugin with NULL name cannot be registered\n");
        return;
    }
    
    if (plugin->process_message == NULL) {
        fprintf(stderr, "Warning: Plugin '%s' has NULL process_message function\n",
                plugin->plugin_name);
        /* Allow registration but warn - plugin will be skipped during processing */
    }
    
    /* Check capacity */
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
            plugin->plugin_name, 
            plugin->plugin_version ? plugin->plugin_version : "unknown");
#endif
}

/* Apply all registered plugins to a message
 * 
 * Processes the message through each registered plugin in sequence.
 * Each plugin receives the output of the previous plugin (or the original
 * message for the first plugin).
 * 
 * ERROR HANDLING:
 * - If a plugin returns NULL, the transformation is skipped (not an error)
 * - The previous message is passed unchanged to the next plugin
 * - Invalid plugins (NULL function pointer) are skipped with a warning
 * 
 * @param message - The original message to transform
 * @return The final transformed message (or original if no transformations applied)
 */
const char* apply_plugins(const char* message) {
    const char* result = message;
    int i;
    
    /* Handle NULL input gracefully */
    if (message == NULL) {
        fprintf(stderr, "Error: apply_plugins called with NULL message\n");
        return NULL;
    }
    
    /* Apply each plugin in registration order */
    for (i = 0; i < plugin_count; i++) {
        /* Skip invalid entries */
        if (plugin_registry[i] == NULL) {
            fprintf(stderr, "Warning: NULL plugin at index %d, skipping\n", i);
            continue;
        }
        
        if (plugin_registry[i]->process_message == NULL) {
            fprintf(stderr, "Warning: Plugin '%s' has NULL process_message, skipping\n",
                    plugin_registry[i]->plugin_name ? plugin_registry[i]->plugin_name : "unknown");
            continue;
        }
        
        /* Call the plugin's transformation function */
        char* transformed = plugin_registry[i]->process_message(result);
        
        if (transformed != NULL) {
            /* Transformation successful, use the result */
            result = transformed;
        }
        /* If transformed is NULL, skip this plugin and continue with previous result */
    }
    
    return result;
}
