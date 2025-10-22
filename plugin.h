/* Plugin System Interface for hello.c
 * Allows external developers to extend functionality without modifying core code
 * 
 * Plugin Architecture:
 * - Plugins can transform the output message before it's printed
 * - Plugins are registered at compile time via the plugin registry
 * - Each plugin implements a transform function that modifies the message
 * - Plugins are chained: output of one becomes input of next
 * 
 * Plugin API:
 * - plugin_transform(): Function signature all plugins must implement
 * - plugin_init(): Initialize the plugin system
 * - plugin_apply_all(): Apply all registered plugins to a message
 * 
 * Example Plugin:
 *   const char* uppercase_transform(const char* input) {
 *       static char buffer[256];
 *       // Transform logic here
 *       return buffer;
 *   }
 * 
 * Usage:
 *   plugin_init();
 *   const char* result = plugin_apply_all("Hello world!");
 *   printf("%s", result);
 */

#ifndef PLUGIN_H
#define PLUGIN_H

/* Maximum number of plugins that can be registered */
#define MAX_PLUGINS 10

/* Maximum buffer size for transformed messages */
#define PLUGIN_BUFFER_SIZE 256

/* Plugin transform function signature
 * 
 * Parameters:
 *   input - The input string to transform (must not be NULL)
 * 
 * Returns:
 *   Pointer to the transformed string. The plugin must ensure the returned
 *   pointer remains valid until the next call to the plugin function.
 *   Typically uses a static buffer or returns the input pointer unchanged.
 * 
 * Thread Safety:
 *   Not thread-safe. Uses static buffers internally.
 */
typedef const char* (*plugin_transform_fn)(const char* input);

/* Plugin descriptor structure */
typedef struct {
    const char* name;              /* Plugin name for identification */
    plugin_transform_fn transform;  /* Transform function */
} plugin_t;

/* Initialize the plugin system
 * Must be called before using any other plugin functions
 */
void plugin_init(void);

/* Register a plugin with the system
 * 
 * Parameters:
 *   plugin - Pointer to plugin descriptor
 * 
 * Returns:
 *   0 on success, -1 if plugin registry is full
 */
int plugin_register(const plugin_t* plugin);

/* Apply all registered plugins to an input string
 * Plugins are applied in the order they were registered
 * 
 * Parameters:
 *   input - The input string to transform
 * 
 * Returns:
 *   Pointer to the final transformed string after all plugins are applied
 */
const char* plugin_apply_all(const char* input);

/* Get the number of registered plugins */
int plugin_count(void);

#endif /* PLUGIN_H */
