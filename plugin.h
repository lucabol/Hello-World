/* Plugin System for hello.c
 * 
 * This header defines the plugin interface for extending hello.c functionality.
 * Plugins can transform the greeting message or add additional features.
 * 
 * Plugin Interface:
 * - plugin_name: Name of the plugin
 * - plugin_version: Version string (e.g., "1.0.0")
 * - plugin_init: Called when the plugin is loaded
 * - plugin_transform: Transform the greeting message
 * - plugin_cleanup: Called before unloading the plugin
 * 
 * Usage:
 * 1. Create a new .c file in the plugins/ directory
 * 2. Implement the plugin_info structure
 * 3. Compile as a shared library (.so on Linux, .dll on Windows)
 * 4. Load the plugin at runtime
 */
#ifndef PLUGIN_H
#define PLUGIN_H

#include <stddef.h>

/* Plugin version for ABI compatibility */
#define PLUGIN_API_VERSION 1

/* Maximum length for plugin names and versions */
#define PLUGIN_NAME_MAX 64
#define PLUGIN_VERSION_MAX 16

/* Plugin function return codes */
#define PLUGIN_SUCCESS 0
#define PLUGIN_ERROR -1

/* Forward declaration */
struct plugin_info;

/* Plugin initialization function
 * Called when the plugin is loaded
 * Returns PLUGIN_SUCCESS on success, PLUGIN_ERROR on failure
 */
typedef int (*plugin_init_fn)(void);

/* Plugin transformation function
 * Transforms the input string and stores result in output buffer
 * 
 * Parameters:
 *   input: The original greeting string (null-terminated)
 *   output: Buffer to store the transformed string
 *   output_size: Size of the output buffer
 * 
 * Returns PLUGIN_SUCCESS on success, PLUGIN_ERROR on failure
 */
typedef int (*plugin_transform_fn)(const char* input, char* output, size_t output_size);

/* Plugin cleanup function
 * Called before the plugin is unloaded
 * Returns PLUGIN_SUCCESS on success, PLUGIN_ERROR on failure
 */
typedef int (*plugin_cleanup_fn)(void);

/* Plugin information structure
 * Each plugin must export a global variable of this type named "plugin_info"
 */
typedef struct plugin_info {
    int api_version;                    /* Must be PLUGIN_API_VERSION */
    char name[PLUGIN_NAME_MAX];         /* Plugin name */
    char version[PLUGIN_VERSION_MAX];   /* Plugin version string */
    plugin_init_fn init;                /* Initialization function (can be NULL) */
    plugin_transform_fn transform;      /* Transformation function (required) */
    plugin_cleanup_fn cleanup;          /* Cleanup function (can be NULL) */
} plugin_info_t;

/* Plugin registry entry */
typedef struct plugin_entry {
    plugin_info_t* info;
    void* handle;  /* For dynamic loading (dlopen handle) */
    struct plugin_entry* next;
} plugin_entry_t;

/* Plugin management functions */

/* Initialize the plugin system */
void plugin_system_init(void);

/* Load a plugin from a shared library file
 * Returns PLUGIN_SUCCESS on success, PLUGIN_ERROR on failure
 */
int plugin_load(const char* path);

/* Register a statically linked plugin
 * Returns PLUGIN_SUCCESS on success, PLUGIN_ERROR on failure
 */
int plugin_register(plugin_info_t* info);

/* Apply all registered plugins to transform the greeting
 * Plugins are applied in the order they were registered
 * Returns PLUGIN_SUCCESS on success, PLUGIN_ERROR on failure
 */
int plugin_apply_all(const char* input, char* output, size_t output_size);

/* Cleanup and unload all plugins */
void plugin_system_cleanup(void);

#endif /* PLUGIN_H */
