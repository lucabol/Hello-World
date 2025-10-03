/* Plugin System for Hello World
 * 
 * This header defines the plugin interface for extending hello.c functionality.
 * External developers can create plugins that transform or modify the greeting
 * message without modifying hello.c directly.
 * 
 * Plugin Interface:
 * - process_message: Transform the greeting message
 * - plugin_name: Name of the plugin for identification
 * - plugin_version: Version string for the plugin
 * 
 * Usage:
 * 1. Create a plugin by implementing the plugin_interface struct
 * 2. Register the plugin using REGISTER_PLUGIN macro
 * 3. Compile your plugin with hello.c
 * 
 * Example Plugin:
 * 
 *   #include "plugin.h"
 *   
 *   static char* my_transform(const char* input) {
 *       static char buffer[256];
 *       snprintf(buffer, sizeof(buffer), "[Modified] %s", input);
 *       return buffer;
 *   }
 *   
 *   static plugin_interface my_plugin = {
 *       .process_message = my_transform,
 *       .plugin_name = "MyPlugin",
 *       .plugin_version = "1.0"
 *   };
 *   
 *   REGISTER_PLUGIN(my_plugin);
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Plugin interface structure */
typedef struct {
    /* Transform the input message and return the result.
     * The returned pointer should point to static storage or
     * memory that remains valid for the duration of the program.
     * Return NULL to indicate no transformation (pass through).
     */
    char* (*process_message)(const char* input);
    
    /* Name of the plugin */
    const char* plugin_name;
    
    /* Version of the plugin */
    const char* plugin_version;
} plugin_interface;

/* Maximum number of plugins that can be registered */
#define MAX_PLUGINS 16

/* Plugin registry - array of plugin pointers */
extern plugin_interface* plugin_registry[MAX_PLUGINS];
extern int plugin_count;

/* Register a plugin - to be called from plugin implementations */
void register_plugin(plugin_interface* plugin);

/* Macro to simplify plugin registration at program initialization */
#define REGISTER_PLUGIN(plugin_var) \
    static void __attribute__((constructor)) register_##plugin_var(void) { \
        register_plugin(&plugin_var); \
    } \
    typedef int register_plugin_dummy_##plugin_var

/* Apply all registered plugins to a message
 * Returns the final transformed message after applying all plugins in order.
 * If no plugins are registered or all return NULL, returns the original input.
 */
const char* apply_plugins(const char* message);

#ifdef __cplusplus
}
#endif

#endif /* PLUGIN_H */
