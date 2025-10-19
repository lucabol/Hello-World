/* Plugin System for hello.c
 * Simple plugin architecture for extending functionality
 * 
 * Plugin Interface:
 * - Plugins can transform the message before output
 * - Multiple plugins can be registered and chained
 * - Plugins are called in registration order
 * 
 * Usage:
 *   1. Define a plugin function: char* my_plugin(const char* input)
 *   2. Register it: register_plugin(my_plugin)
 *   3. Plugin will be called automatically when message is printed
 */

#ifndef PLUGIN_H
#define PLUGIN_H

/* Maximum number of plugins that can be registered */
#define MAX_PLUGINS 10

/* Plugin function type - transforms input string and returns new string
 * Input: const char* - the input message
 * Return: char* - the transformed message (must be malloc'd if modified)
 * Note: Plugin must return a valid string. If no transformation is needed,
 *       return the input pointer directly (do not malloc).
 */
typedef char* (*plugin_func)(const char* input);

/* Register a plugin function
 * Returns: 0 on success, -1 if plugin array is full
 */
int register_plugin(plugin_func func);

/* Apply all registered plugins to the input string
 * Returns: transformed string (caller must free if different from input)
 */
char* apply_plugins(const char* input);

/* Clear all registered plugins */
void clear_plugins(void);

/* Get number of registered plugins */
int get_plugin_count(void);

#endif /* PLUGIN_H */
