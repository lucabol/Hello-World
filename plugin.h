/* Plugin Interface for hello.c
 * SPDX-License-Identifier: MIT
 * 
 * This header defines the plugin architecture for extending hello.c functionality
 * without modifying the core file directly.
 * 
 * Thread Safety:
 * - ⚠️ NOT THREAD-SAFE: This API is NOT thread-safe. All plugin operations
 *   (register, unregister, apply, clear) must be performed from a single thread
 *   or protected by external synchronization.
 * - Rationale: Static registry without locking for simplicity and performance in
 *   single-threaded use cases (the most common scenario).
 * - Multi-threaded use: Callers must provide external synchronization:
 *     pthread_mutex_lock(&your_mutex);
 *     plugin_register(&my_plugin);  // or any other plugin_* call
 *     pthread_mutex_unlock(&your_mutex);
 * - ⚠️ WARNING: Calling plugin_apply_all() while another thread is modifying the
 *   registry (register/unregister/clear) results in undefined behavior.
 * - ⚠️ WARNING: Unregistering a plugin during plugin_apply_all() is undefined.
 * 
 * Registry Capacity:
 * - Maximum plugins: PLUGIN_MAX_COUNT (default 10, configurable at compile time)
 * - Query capacity: Use plugin_get_count() to check current count
 * - plugin_register() returns PLUGIN_ERR_REGISTRY_FULL when capacity is reached
 * 
 * Plugin Contract:
 * - Plugins implement a transform function that processes a message
 * - Plugins have a name for identification
 * - Plugins can be chained together
 * - Multiple plugins can be registered and executed in sequence
 * - Plugins are applied in the order they were registered
 * - Plugin indices are NOT stable across unregister/register operations
 * 
 * Memory/Ownership:
 * - Caller owns plugin_t structures: The plugin system does NOT copy or free them
 * - plugin_t instances must remain valid for the lifetime of their registration
 * - Typically declare plugins as static/global variables
 * - Plugin names must remain valid (use string literals or static storage)
 * - Before freeing/unloading plugin code, call plugin_unregister() first
 * - Undefined behavior if a registered plugin's memory is freed/unmapped
 * 
 * Error Handling:
 * - If a plugin returns non-zero, plugin_apply_all() stops immediately
 * - Returns PLUGIN_ERR_TRANSFORM_FAILED to caller
 * - Output buffer contains result of last successful transformation
 * - Callers should check return values and handle errors appropriately
 * 
 * Buffer Handling:
 * - Output is always NUL-terminated when buffer size >= 1
 * - If output would overflow, returns PLUGIN_ERR_BUFFER_TOO_SMALL
 * - No out-of-bounds writes are ever performed
 * 
 * Usage:
 *   1. Implement a plugin by defining a transform function
 *   2. Create a plugin_t structure with name and function pointer
 *   3. Register the plugin using plugin_register()
 *   4. The plugin system will apply all registered plugins in order
 *   5. Use plugin_unregister() to remove specific plugins
 *   6. Use plugin_clear() to reset the entire registry
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Plugin API Version - increment on breaking changes */
#define PLUGIN_API_VERSION_MAJOR 1
#define PLUGIN_API_VERSION_MINOR 0
#define PLUGIN_API_VERSION_PATCH 0

/* Maximum message length supported by plugins */
#define PLUGIN_MAX_MESSAGE_LEN 256

/* Maximum number of plugins that can be registered
 * This can be overridden at compile time: -DPLUGIN_MAX_COUNT=20
 */
#ifndef PLUGIN_MAX_COUNT
#define PLUGIN_MAX_COUNT 10
#endif

/* Error codes returned by plugin API functions */
typedef enum {
    PLUGIN_SUCCESS = 0,              /* Operation succeeded */
    PLUGIN_ERR_NULL_POINTER = -1,    /* NULL pointer argument */
    PLUGIN_ERR_INVALID_PLUGIN = -2,  /* Invalid plugin structure */
    PLUGIN_ERR_REGISTRY_FULL = -3,   /* Plugin registry is full */
    PLUGIN_ERR_BUFFER_TOO_SMALL = -4, /* Output buffer too small */
    PLUGIN_ERR_TRANSFORM_FAILED = -5, /* Plugin transform function failed */
    PLUGIN_ERR_PLUGIN_NOT_FOUND = -6  /* Plugin not found in registry */
} plugin_error_t;

/* Plugin transform function signature
 * 
 * Parameters:
 *   input: The input message to transform (null-terminated, never NULL)
 *   output: Buffer to write the transformed message (never NULL)
 *   output_size: Size of the output buffer (always > 0)
 * 
 * Returns:
 *   PLUGIN_SUCCESS (0) on success, or a negative error code
 * 
 * Contract:
 *   - Must not write beyond output_size bytes
 *   - Must null-terminate the output string on success
 *   - Input is guaranteed to be non-NULL and null-terminated
 *   - Output buffer is guaranteed to be non-NULL with size > 0
 *   - Should return PLUGIN_ERR_BUFFER_TOO_SMALL if output won't fit
 */
typedef int (*plugin_transform_fn)(const char* input, char* output, size_t output_size);

/* Plugin structure
 * 
 * All fields must be non-NULL for a valid plugin.
 * The structure and name must remain valid for the lifetime of registration.
 */
typedef struct {
    const char* name;              /* Plugin name for identification (must be non-NULL) */
    plugin_transform_fn transform; /* Transform function (must be non-NULL) */
} plugin_t;

/* Register a plugin
 * 
 * Parameters:
 *   plugin: Pointer to plugin structure (must be non-NULL)
 * 
 * Returns:
 *   PLUGIN_SUCCESS on success
 *   PLUGIN_ERR_NULL_POINTER if plugin is NULL
 *   PLUGIN_ERR_INVALID_PLUGIN if plugin->name or plugin->transform is NULL
 *   PLUGIN_ERR_REGISTRY_FULL if maximum plugins already registered
 * 
 * Thread Safety: Not thread-safe
 * Ownership: Caller retains ownership of plugin structure
 */
int plugin_register(plugin_t* plugin);

/* Unregister a plugin by name
 * 
 * Parameters:
 *   name: Name of the plugin to unregister (must be non-NULL)
 * 
 * Returns:
 *   PLUGIN_SUCCESS on success
 *   PLUGIN_ERR_NULL_POINTER if name is NULL
 *   PLUGIN_ERR_PLUGIN_NOT_FOUND if no plugin with that name exists
 * 
 * Thread Safety: Not thread-safe
 */
int plugin_unregister(const char* name);

/* Apply all registered plugins to a message
 * 
 * Plugins are applied in the order they were registered. If a plugin
 * returns non-zero, processing stops immediately and the error is returned.
 * The output will contain the result of the last successful transformation.
 * 
 * Buffer Guarantees:
 * - Output is always NUL-terminated when output_size >= 1
 * - No out-of-bounds writes are ever performed
 * - If buffer is too small, returns PLUGIN_ERR_BUFFER_TOO_SMALL
 * 
 * Parameters:
 *   input: The input message (must be non-NULL, null-terminated)
 *   output: Buffer to write the final transformed message (must be non-NULL)
 *   output_size: Size of the output buffer (must be > 0)
 * 
 * Returns:
 *   >= 0: Number of plugins successfully applied (0 if no plugins registered)
 *   PLUGIN_ERR_NULL_POINTER (-1): input or output is NULL
 *   PLUGIN_ERR_BUFFER_TOO_SMALL (-4): output_size is 0 or buffer too small
 *   PLUGIN_ERR_TRANSFORM_FAILED (-5): a plugin transformation failed
 * 
 * Thread Safety: Not thread-safe
 * ⚠️ WARNING: Calling plugin_unregister() or plugin_clear() during this
 * function's execution from another thread results in undefined behavior.
 */
int plugin_apply_all(const char* input, char* output, size_t output_size);

/* Get the number of registered plugins
 * 
 * Returns:
 *   Number of currently registered plugins (>= 0)
 * 
 * Thread Safety: Not thread-safe (read-only but no synchronization)
 */
int plugin_get_count(void);

/* Get the maximum plugin capacity
 * 
 * Returns:
 *   Maximum number of plugins that can be registered (PLUGIN_MAX_COUNT)
 * 
 * Thread Safety: Thread-safe (returns compile-time constant)
 */
int plugin_get_capacity(void);

/* Clear all registered plugins
 * 
 * Removes all plugins from the registry. Does not free plugin structures.
 * Useful for test cleanup and resetting state.
 * 
 * Thread Safety: Not thread-safe
 */
void plugin_clear(void);

/* Legacy API compatibility - these call the new prefixed functions */
#define register_plugin(p) plugin_register(p)
#define apply_plugins(i, o, s) plugin_apply_all(i, o, s)
#define get_plugin_count() plugin_get_count()
#define clear_plugins() plugin_clear()

#ifdef __cplusplus
}
#endif

#endif /* PLUGIN_H */
