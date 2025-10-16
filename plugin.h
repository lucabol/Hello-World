/* Plugin System for hello.c
 * Allows external developers to extend functionality without modifying core code
 * 
 * Plugin API:
 * - Plugins can register message transformers
 * - Plugins are loaded at runtime
 * - Multiple plugins can be chained
 * - Original behavior is preserved when no plugins are loaded
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#include <stddef.h>

/* Maximum number of plugins that can be registered */
#define MAX_PLUGINS 10

/* Plugin callback function type
 * Takes input message and buffer, returns transformed message
 * Parameters:
 *   input: Original message to transform
 *   output: Buffer to write transformed message (must be at least 256 bytes)
 *   output_size: Size of output buffer
 * Returns: pointer to output buffer or NULL on error
 */
typedef char* (*plugin_transform_func)(const char* input, char* output, size_t output_size);

/* Plugin structure */
typedef struct {
    const char* name;                    /* Plugin name */
    const char* description;             /* Plugin description */
    plugin_transform_func transform;     /* Transformation function */
} plugin_t;

/* Register a plugin
 * Returns: 0 on success, -1 on failure (too many plugins)
 */
int plugin_register(const char* name, const char* description, plugin_transform_func transform);

/* Apply all registered plugins to a message
 * Parameters:
 *   input: Original message
 *   output: Buffer for final output (must be at least 256 bytes)
 *   output_size: Size of output buffer
 * Returns: pointer to output buffer
 */
char* plugin_apply_all(const char* input, char* output, size_t output_size);

/* Get number of registered plugins */
int plugin_count(void);

/* Initialize plugin system (called automatically) */
void plugin_init(void);

#endif /* PLUGIN_H */
