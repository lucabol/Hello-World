/* Plugin System Implementation
 * Manages plugin registration and execution
 * 
 * Implementation Notes:
 * - Uses two staging buffers (ping-pong) to safely chain plugins
 * - Even when plugins return pointers to their own static buffers,
 *   the staging mechanism prevents aliasing issues
 * - All plugin outputs are copied before calling the next plugin
 */

#include "plugin.h"
#include <string.h>

/* Plugin registry - stores all registered plugins */
static const plugin_t* plugin_registry[PLUGIN_MAX_PLUGINS];
static int plugin_registry_count = 0;

/* Dual staging buffers for safe plugin chaining (ping-pong buffering)
 * We alternate between these two buffers when chaining plugins to ensure
 * that even if a plugin returns a pointer to its own static buffer that
 * might overlap with our staging buffer, we have a safe copy */
static char plugin_staging_buffer_a[PLUGIN_BUFFER_SIZE];
static char plugin_staging_buffer_b[PLUGIN_BUFFER_SIZE];

/* Initialize the plugin system */
void plugin_init(void) {
    int i;
    plugin_registry_count = 0;
    
    /* Clear registry */
    for (i = 0; i < PLUGIN_MAX_PLUGINS; i++) {
        plugin_registry[i] = NULL;
    }
    
    /* Clear staging buffers */
    memset(plugin_staging_buffer_a, 0, PLUGIN_BUFFER_SIZE);
    memset(plugin_staging_buffer_b, 0, PLUGIN_BUFFER_SIZE);
}

/* Register a plugin */
int plugin_register(const plugin_t* plugin) {
    int i;
    
    /* Validate plugin pointer and contents */
    if (plugin == NULL) {
        return PLUGIN_ERROR_NULL;
    }
    
    if (plugin->name == NULL || plugin->transform == NULL) {
        return PLUGIN_ERROR_NULL;
    }
    
    /* Check if registry is full */
    if (plugin_registry_count >= PLUGIN_MAX_PLUGINS) {
        return PLUGIN_ERROR_FULL;
    }
    
    /* Check for duplicate registration (same pointer) */
    for (i = 0; i < plugin_registry_count; i++) {
        if (plugin_registry[i] == plugin) {
            return PLUGIN_ERROR_DUPLICATE;
        }
    }
    
    /* Register the plugin */
    plugin_registry[plugin_registry_count] = plugin;
    plugin_registry_count++;
    return PLUGIN_SUCCESS;
}

/* Apply all registered plugins to input string */
const char* plugin_apply_all(const char* input) {
    const char* current = input;
    char* current_buffer;
    char* next_buffer;
    int i;
    size_t len;
    
    /* Validate input */
    if (input == NULL) {
        return NULL;
    }
    
    /* If no plugins registered, return input unchanged */
    if (plugin_registry_count == 0) {
        return input;
    }
    
    /* Start with buffer A */
    current_buffer = plugin_staging_buffer_a;
    next_buffer = plugin_staging_buffer_b;
    
    /* Apply each plugin in sequence */
    for (i = 0; i < plugin_registry_count; i++) {
        const char* plugin_result;
        
        /* Validate plugin before calling */
        if (plugin_registry[i] == NULL || plugin_registry[i]->transform == NULL) {
            return NULL;  /* Invalid plugin in registry */
        }
        
        /* Call the plugin transform function */
        plugin_result = plugin_registry[i]->transform(current);
        
        /* Check for plugin error */
        if (plugin_result == NULL) {
            return NULL;  /* Plugin signaled an error */
        }
        
        /* Check result length and copy to staging buffer */
        len = strlen(plugin_result);
        if (len >= PLUGIN_BUFFER_SIZE) {
            return NULL;  /* Result too large */
        }
        
        /* Copy result to next staging buffer
         * Use memcpy for efficiency since we know the exact length
         * This copy ensures safe chaining even if plugin_result points
         * to a static buffer that might overlap with current_buffer */
        memcpy(next_buffer, plugin_result, len + 1);  /* +1 for null terminator */
        
        /* Update current to point to the buffer we just filled */
        current = next_buffer;
        
        /* Swap buffers for next iteration (ping-pong) */
        {
            char* temp = current_buffer;
            current_buffer = next_buffer;
            next_buffer = temp;
        }
    }
    
    /* Return pointer to the final result
     * This will be either staging_buffer_a or staging_buffer_b
     * depending on the number of plugins */
    return current;
}

/* Get the number of registered plugins */
int plugin_count(void) {
    return plugin_registry_count;
}
