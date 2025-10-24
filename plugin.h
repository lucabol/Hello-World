/* Plugin Interface for hello.c
 * 
 * This header defines the plugin architecture for extending hello.c functionality
 * without modifying the core file directly.
 * 
 * Plugin Contract:
 * - Plugins implement a transform function that processes a message
 * - Plugins have a name for identification
 * - Plugins can be chained together
 * - Multiple plugins can be registered and executed in sequence
 * 
 * Usage:
 *   1. Implement a plugin by defining a transform function
 *   2. Create a plugin_t structure with name and function pointer
 *   3. Register the plugin using register_plugin()
 *   4. The plugin system will apply all registered plugins in order
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#include <stddef.h>

/* Maximum message length supported by plugins */
#define PLUGIN_MAX_MESSAGE_LEN 256

/* Maximum number of plugins that can be registered */
#define PLUGIN_MAX_COUNT 10

/* Plugin transform function signature
 * 
 * Parameters:
 *   input: The input message to transform (null-terminated)
 *   output: Buffer to write the transformed message (must be PLUGIN_MAX_MESSAGE_LEN)
 *   output_size: Size of the output buffer
 * 
 * Returns:
 *   0 on success, non-zero on error
 * 
 * Contract:
 *   - Must not write beyond output_size bytes
 *   - Must null-terminate the output string
 *   - Should handle NULL input gracefully
 */
typedef int (*plugin_transform_fn)(const char* input, char* output, size_t output_size);

/* Plugin structure */
typedef struct {
    const char* name;              /* Plugin name for identification */
    plugin_transform_fn transform; /* Transform function */
} plugin_t;

/* Register a plugin
 * Returns: 0 on success, -1 if plugin registry is full
 */
int register_plugin(plugin_t* plugin);

/* Apply all registered plugins to a message
 * Plugins are applied in the order they were registered
 * 
 * Parameters:
 *   input: The input message
 *   output: Buffer to write the final transformed message
 *   output_size: Size of the output buffer
 * 
 * Returns:
 *   Number of plugins applied, or -1 on error
 */
int apply_plugins(const char* input, char* output, size_t output_size);

/* Get the number of registered plugins */
int get_plugin_count(void);

/* Clear all registered plugins */
void clear_plugins(void);

#endif /* PLUGIN_H */
