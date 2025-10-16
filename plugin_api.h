/* Plugin API for Hello World Program
 * 
 * This header defines the interface that plugins must implement to extend
 * the Hello World program's functionality. Plugins can transform the output
 * message before it is displayed.
 * 
 * Plugin Contract:
 * - Plugins must implement the plugin_init() and plugin_transform() functions
 * - plugin_init() is called once when the plugin is loaded
 * - plugin_transform() receives the current message and returns a transformed version
 * - Plugins must return heap-allocated strings from plugin_transform()
 * - The caller is responsible for freeing the returned string
 * 
 * Example Plugin:
 * 
 *   #include "plugin_api.h"
 *   #include <string.h>
 *   #include <stdlib.h>
 * 
 *   int plugin_init(void) {
 *       return 0;  // Success
 *   }
 * 
 *   char* plugin_transform(const char* input) {
 *       char* output = malloc(strlen(input) + 10);
 *       sprintf(output, "[%s]", input);
 *       return output;
 *   }
 */

#ifndef PLUGIN_API_H
#define PLUGIN_API_H

/* Plugin initialization function
 * Called once when plugin is loaded
 * Returns: 0 on success, non-zero on failure
 */
typedef int (*plugin_init_func)(void);

/* Plugin transformation function
 * Transforms the input message and returns a new message
 * Parameters:
 *   input - The current message (null-terminated string)
 * Returns: A heap-allocated transformed message (caller must free)
 *          Returns NULL on error
 */
typedef char* (*plugin_transform_func)(const char* input);

/* Function names that plugins must export */
#define PLUGIN_INIT_FUNC "plugin_init"
#define PLUGIN_TRANSFORM_FUNC "plugin_transform"

#endif /* PLUGIN_API_H */
