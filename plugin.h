/* Plugin API for hello.c extensibility
 * 
 * This header defines the plugin interface for extending hello.c functionality.
 * Plugins can transform the greeting message before it's displayed.
 * 
 * Plugin Contract:
 * - Plugins must implement the plugin_transform() function
 * - Input: original message (read-only)
 * - Output: transformed message (dynamically allocated or static)
 * - Plugins must NOT free the input message
 * - Plugins are responsible for memory management of returned strings
 * 
 * Plugin Loading:
 * - Plugins are loaded dynamically at runtime via dlopen()
 * - Set HELLO_PLUGIN environment variable to plugin path
 * - Example: HELLO_PLUGIN=./plugins/uppercase.so ./hello
 * 
 * Building Plugins:
 * - Compile as shared library: gcc -shared -fPIC -o plugin.so plugin.c
 * - Link with -ldl when building main program
 */

#ifndef PLUGIN_H
#define PLUGIN_H

/* Plugin API version for compatibility checking */
#define PLUGIN_API_VERSION 1

/* Plugin metadata structure */
typedef struct {
    const char* name;           /* Plugin name */
    const char* version;        /* Plugin version */
    const char* description;    /* Brief description */
    int api_version;            /* API version this plugin uses */
} PluginInfo;

/* Plugin transformation function signature
 * 
 * Parameters:
 *   message - Original message to transform (read-only, do not free)
 * 
 * Returns:
 *   Transformed message (caller does not need to free if static)
 *   Return NULL on error
 * 
 * Notes:
 *   - Must be thread-safe if multiple calls are possible
 *   - Should handle NULL input gracefully
 *   - Should be efficient (called on every message output)
 */
typedef const char* (*plugin_transform_func)(const char* message);

/* Plugin initialization function signature (optional)
 * Called once when plugin is loaded
 * Returns 0 on success, non-zero on failure
 */
typedef int (*plugin_init_func)(void);

/* Plugin cleanup function signature (optional)
 * Called once before plugin is unloaded
 */
typedef void (*plugin_cleanup_func)(void);

/* Plugin info function signature
 * Returns metadata about the plugin
 */
typedef PluginInfo (*plugin_info_func)(void);

/* Standard plugin function names that plugins must export */
#define PLUGIN_TRANSFORM_NAME "plugin_transform"
#define PLUGIN_INFO_NAME "plugin_info"
#define PLUGIN_INIT_NAME "plugin_init"       /* Optional */
#define PLUGIN_CLEANUP_NAME "plugin_cleanup" /* Optional */

#endif /* PLUGIN_H */
