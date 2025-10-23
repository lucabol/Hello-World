/* Plugin System Interface for hello.c
 * Allows external developers to extend functionality without modifying core code
 * 
 * Plugin Architecture:
 * - Plugins can transform the output message before it's printed
 * - Plugins are registered at runtime via the plugin registry
 * - Each plugin implements a transform function that modifies the message
 * - Plugins are chained: output of one becomes input of next
 * - Internal staging buffers ensure safe chaining even when plugins use static buffers
 * 
 * Thread Safety:
 * - This implementation is NOT thread-safe
 * - Uses static buffers internally for performance and simplicity
 * - Do not call plugin functions from multiple threads concurrently
 * - Do not call plugin functions reentrantly (e.g., from signal handlers)
 * 
 * Buffer Ownership and Lifetime:
 * - Plugins return const char* pointers to transformed strings
 * - Returned pointers may point to plugin-internal static buffers
 * - plugin_apply_all() copies each plugin's output to an internal staging buffer
 *   before calling the next plugin, ensuring safe chaining
 * - The final result from plugin_apply_all() points to an internal static buffer
 * - The returned pointer remains valid until the next call to plugin_apply_all()
 * - Callers must copy the result if they need it to persist longer
 * 
 * Plugin API:
 * - plugin_init(): Initialize the plugin system
 * - plugin_register(): Register a plugin for use
 * - plugin_apply_all(): Apply all registered plugins to a message
 * - plugin_count(): Get number of registered plugins
 * 
 * Example Plugin:
 *   const char* uppercase_transform(const char* input) {
 *       static char buffer[PLUGIN_BUFFER_SIZE];
 *       size_t len = strlen(input);
 *       if (len >= PLUGIN_BUFFER_SIZE) len = PLUGIN_BUFFER_SIZE - 1;
 *       for (size_t i = 0; i < len; i++) {
 *           buffer[i] = (char)toupper((unsigned char)input[i]);
 *       }
 *       buffer[len] = '\0';
 *       return buffer;
 *   }
 * 
 * Usage:
 *   plugin_init();
 *   if (plugin_register(&my_plugin) != 0) {
 *       // Handle error (registry full, NULL plugin, etc.)
 *   }
 *   const char* result = plugin_apply_all("Hello world!");
 *   if (result == NULL) {
 *       // Handle error (plugin returned NULL, buffer overflow, etc.)
 *   }
 *   printf("%s", result);
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#include <stddef.h>  /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum number of plugins that can be registered
 * Override at compile time with -DPLUGIN_MAX_PLUGINS=N if needed */
#ifndef PLUGIN_MAX_PLUGINS
#define PLUGIN_MAX_PLUGINS 10
#endif

/* Maximum buffer size for transformed messages
 * Override at compile time with -DPLUGIN_BUFFER_SIZE=N if needed */
#ifndef PLUGIN_BUFFER_SIZE
#define PLUGIN_BUFFER_SIZE 256
#endif

/* Legacy compatibility - will be deprecated */
#define MAX_PLUGINS PLUGIN_MAX_PLUGINS

/* Plugin transform function signature
 * 
 * Parameters:
 *   input - The input string to transform (must not be NULL)
 * 
 * Returns:
 *   Pointer to the transformed string, or NULL on error
 *   
 *   The returned pointer typically points to a plugin-internal static buffer.
 *   The plugin system (plugin_apply_all) will copy this result to an internal
 *   staging buffer before calling the next plugin, ensuring safe chaining.
 *   
 *   The returned string must be null-terminated and should not exceed
 *   PLUGIN_BUFFER_SIZE - 1 bytes (excluding null terminator).
 *   
 *   If transformation fails or input is invalid, return NULL to signal error.
 * 
 * Thread Safety:
 *   Not thread-safe. Plugins typically use static buffers.
 * 
 * Lifetime:
 *   The returned pointer must remain valid at least until this function
 *   is called again. Static buffers satisfy this requirement.
 */
typedef const char* (*plugin_transform_fn)(const char* input);

/* Plugin descriptor structure */
typedef struct {
    const char* name;              /* Plugin name for identification (must not be NULL) */
    plugin_transform_fn transform;  /* Transform function (must not be NULL) */
} plugin_t;

/* Plugin registration error codes */
#define PLUGIN_SUCCESS          0   /* Operation succeeded */
#define PLUGIN_ERROR_FULL      -1   /* Plugin registry is full */
#define PLUGIN_ERROR_NULL      -2   /* NULL plugin or invalid plugin descriptor */
#define PLUGIN_ERROR_DUPLICATE -3   /* Plugin already registered */

/* Plugin execution error codes (for plugin_apply_all failures) */
#define PLUGIN_ERROR_INPUT_NULL     -4   /* Input to plugin_apply_all was NULL */
#define PLUGIN_ERROR_PLUGIN_FAILED  -5   /* A plugin returned NULL during transformation */
#define PLUGIN_ERROR_OUTPUT_TOO_LARGE -6 /* Plugin output exceeded buffer size */
#define PLUGIN_ERROR_INVALID_PLUGIN -7   /* Plugin in registry has NULL transform function */

/* Initialize the plugin system
 * Must be called before using any other plugin functions
 * Can be called multiple times to reset the system
 */
void plugin_init(void);

/* Register a plugin with the system
 * 
 * Parameters:
 *   plugin - Pointer to plugin descriptor (must not be NULL)
 *            plugin->name and plugin->transform must not be NULL
 * 
 * Returns:
 *   PLUGIN_SUCCESS (0) on success
 *   PLUGIN_ERROR_FULL (-1) if plugin registry is full
 *   PLUGIN_ERROR_NULL (-2) if plugin pointer, name, or transform is NULL
 *   PLUGIN_ERROR_DUPLICATE (-3) if plugin is already registered
 * 
 * Notes:
 *   - Duplicate detection is based on pointer equality (same plugin_t* address)
 *   - Different plugin_t instances with the same name are allowed
 *   - The same plugin_t pointer cannot be registered twice
 *   - Registration order determines execution order in plugin_apply_all()
 * 
 * Example:
 *   plugin_t p1 = {.name = "foo", .transform = fn1};
 *   plugin_t p2 = {.name = "foo", .transform = fn2};
 *   plugin_register(&p1);  // OK
 *   plugin_register(&p2);  // OK (different pointer, even though same name)
 *   plugin_register(&p1);  // PLUGIN_ERROR_DUPLICATE (same pointer)
 */
int plugin_register(const plugin_t* plugin);

/* Apply all registered plugins to an input string
 * Plugins are applied in the order they were registered
 * 
 * Parameters:
 *   input - The input string to transform (must not be NULL)
 * 
 * Returns:
 *   Pointer to the final transformed string, or NULL on error
 *   
 *   The returned pointer points to an internal static buffer and remains
 *   valid until the next call to plugin_apply_all().
 *   
 *   If you need the result to persist longer, copy it to your own buffer.
 *   
 *   Returns NULL if:
 *   - input is NULL
 *   - any plugin returns NULL (error in transformation)
 *   - result would exceed PLUGIN_BUFFER_SIZE - 1 bytes
 * 
 * Implementation:
 *   Uses two internal staging buffers to safely chain plugins.
 *   Each plugin's output is copied to a staging buffer before calling
 *   the next plugin, preventing aliasing issues when plugins use
 *   static buffers that might overlap.
 * 
 * Thread Safety:
 *   Not thread-safe. Do not call from multiple threads concurrently.
 */
const char* plugin_apply_all(const char* input);

/* Get the number of registered plugins 
 * 
 * Returns:
 *   Number of currently registered plugins (0 to PLUGIN_MAX_PLUGINS)
 */
int plugin_count(void);

/* Get the last error code from plugin operations
 * 
 * This function returns the error code from the most recent plugin operation
 * that failed. It's particularly useful after plugin_apply_all() returns NULL
 * to determine the specific cause of failure.
 * 
 * Returns:
 *   PLUGIN_SUCCESS if the last operation succeeded
 *   PLUGIN_ERROR_INPUT_NULL if input was NULL
 *   PLUGIN_ERROR_PLUGIN_FAILED if a plugin returned NULL
 *   PLUGIN_ERROR_OUTPUT_TOO_LARGE if output exceeded buffer size
 *   PLUGIN_ERROR_INVALID_PLUGIN if a plugin has NULL transform function
 *   Or any registration error code from plugin_register()
 * 
 * Thread Safety:
 *   Not thread-safe. Uses static storage for error code.
 * 
 * Example:
 *   const char* result = plugin_apply_all("input");
 *   if (result == NULL) {
 *       int error = plugin_get_last_error();
 *       switch (error) {
 *           case PLUGIN_ERROR_INPUT_NULL:
 *               fprintf(stderr, "Input was NULL\n");
 *               break;
 *           case PLUGIN_ERROR_PLUGIN_FAILED:
 *               fprintf(stderr, "A plugin failed\n");
 *               break;
 *           // ... handle other errors
 *       }
 *   }
 */
int plugin_get_last_error(void);

#ifdef __cplusplus
}
#endif

#endif /* PLUGIN_H */
