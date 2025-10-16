/* Plugin Loader API
 * 
 * Provides functionality to load and manage plugins for the Hello World program.
 * Supports loading plugins from shared libraries (.so on Linux, .dylib on macOS, .dll on Windows)
 * 
 * Usage:
 *   1. Call plugin_loader_init() to initialize the plugin system
 *   2. Call plugin_loader_load() for each plugin library to load
 *   3. Call plugin_loader_apply_transforms() to apply all loaded plugins to a message
 *   4. Call plugin_loader_cleanup() to unload all plugins and free resources
 */

#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include "plugin_api.h"

/* Maximum number of plugins that can be loaded simultaneously */
#define MAX_PLUGINS 32

/* Initialize the plugin loader system
 * Must be called before any other plugin_loader functions
 * Returns: 0 on success, non-zero on failure
 */
int plugin_loader_init(void);

/* Load a plugin from a shared library file
 * Parameters:
 *   plugin_path - Path to the plugin shared library file
 * Returns: 0 on success, non-zero on failure
 */
int plugin_loader_load(const char* plugin_path);

/* Apply all loaded plugins to transform a message
 * Plugins are applied in the order they were loaded
 * Parameters:
 *   input - The original message
 * Returns: A heap-allocated transformed message (caller must free)
 *          Returns a copy of input if no plugins are loaded
 *          Returns NULL on error
 */
char* plugin_loader_apply_transforms(const char* input);

/* Get the number of currently loaded plugins
 * Returns: Number of loaded plugins
 */
int plugin_loader_get_count(void);

/* Cleanup and unload all plugins
 * Should be called before program exit
 */
void plugin_loader_cleanup(void);

#endif /* PLUGIN_LOADER_H */
