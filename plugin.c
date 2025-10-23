/* Plugin System Implementation
 * Provides the runtime infrastructure for the plugin system
 */

#include "plugin.h"
#include <stdio.h>
#include <string.h>

/* Global plugin registry */
plugin_t plugin_registry[MAX_PLUGINS];
int plugin_count = 0;

/* Register a plugin */
void plugin_register(const char* name, 
                    plugin_transform_fn transform,
                    plugin_hook_fn before,
                    plugin_hook_fn after) {
    if (plugin_count >= MAX_PLUGINS) {
        fprintf(stderr, "Warning: Maximum number of plugins (%d) reached. Cannot register '%s'\n", 
                MAX_PLUGINS, name);
        return;
    }
    
    plugin_registry[plugin_count].name = name;
    plugin_registry[plugin_count].transform = transform;
    plugin_registry[plugin_count].before = before;
    plugin_registry[plugin_count].after = after;
    plugin_count++;
}

/* Execute all transform functions in registered order */
const char* plugin_execute_transforms(const char* input) {
    const char* result = input;
    int i;
    
    for (i = 0; i < plugin_count; i++) {
        if (plugin_registry[i].transform != NULL) {
            result = plugin_registry[i].transform(result);
        }
    }
    
    return result;
}

/* Execute all before hooks */
void plugin_execute_before_hooks(void) {
    int i;
    for (i = 0; i < plugin_count; i++) {
        if (plugin_registry[i].before != NULL) {
            plugin_registry[i].before();
        }
    }
}

/* Execute all after hooks */
void plugin_execute_after_hooks(void) {
    int i;
    for (i = 0; i < plugin_count; i++) {
        if (plugin_registry[i].after != NULL) {
            plugin_registry[i].after();
        }
    }
}
