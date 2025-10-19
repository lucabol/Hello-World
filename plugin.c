/* Plugin System Implementation */

#include "plugin.h"
#include <stdlib.h>
#include <string.h>

/* Array to store registered plugins */
static plugin_func plugins[MAX_PLUGINS];
static int plugin_count = 0;

/* Register a plugin function */
int register_plugin(plugin_func func) {
    if (plugin_count >= MAX_PLUGINS || func == NULL) {
        return -1;
    }
    plugins[plugin_count] = func;
    plugin_count++;
    return 0;
}

/* Apply all registered plugins to the input string */
char* apply_plugins(const char* input) {
    if (input == NULL || plugin_count == 0) {
        return (char*)input;
    }
    
    char* current = (char*)input;
    char* previous = NULL;
    
    for (int i = 0; i < plugin_count; i++) {
        char* result = plugins[i](current);
        
        /* Free the previous intermediate result if it's not the original input */
        if (previous != NULL && previous != input) {
            free(previous);
        }
        
        previous = current;
        current = result;
    }
    
    return current;
}

/* Clear all registered plugins */
void clear_plugins(void) {
    plugin_count = 0;
}

/* Get number of registered plugins */
int get_plugin_count(void) {
    return plugin_count;
}
