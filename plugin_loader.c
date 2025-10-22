/* Plugin loader implementation
 * Handles applying plugin transformations in sequence
 */

#include "plugin.h"
#include <stddef.h>

/* Apply all registered plugins to input string
 * Each plugin receives the output from the previous plugin
 * Returns final transformed string
 */
const char* apply_plugins(const char* input, const plugin_t* plugins, int plugin_count) {
    const char* current = input;
    int i;
    
    /* If no plugins, return input unchanged */
    if (plugins == NULL || plugin_count == 0) {
        return input;
    }
    
    /* Apply each plugin in sequence */
    for (i = 0; i < plugin_count; i++) {
        if (plugins[i].transform != NULL) {
            current = plugins[i].transform(current);
        }
    }
    
    return current;
}
