/* Plugin System for hello.c
 * Provides a simple plugin architecture for extending functionality
 * 
 * Plugin Interface:
 * - Plugins can transform the greeting message before it's printed
 * - Multiple plugins can be chained together
 * - Each plugin receives input string and returns transformed string
 * 
 * Usage:
 *   1. Define a plugin function: const char* my_plugin(const char* input)
 *   2. Register plugin by adding it to the plugin array in hello.c
 *   3. Compile hello.c with plugin source files
 * 
 * Example Plugin:
 *   const char* uppercase_plugin(const char* input) {
 *       static char buffer[256];
 *       // Transform input to uppercase and store in buffer
 *       return buffer;
 *   }
 */

#ifndef PLUGIN_H
#define PLUGIN_H

/* Plugin function signature
 * Takes an input string and returns a transformed string
 * Plugin must return a pointer to static or heap memory
 * Plugin must NOT modify the input string
 */
typedef const char* (*plugin_function)(const char* input);

/* Plugin metadata structure */
typedef struct {
    const char* name;           /* Plugin name for identification */
    const char* description;    /* Brief description of what plugin does */
    plugin_function transform;  /* Plugin transformation function */
} plugin_t;

/* Apply all registered plugins to input string
 * Returns the final transformed string after all plugins are applied
 * If no plugins are registered, returns input unchanged
 */
const char* apply_plugins(const char* input, const plugin_t* plugins, int plugin_count);

#endif /* PLUGIN_H */
