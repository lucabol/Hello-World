#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "plugin.h"

// Plugin registry storage
static const plugin_t* registered_plugins[MAX_PLUGINS];
static int plugin_count = 0;
static char output_buffer[1024]; // Buffer for transformed output

/**
 * Register a plugin with the system
 */
int plugin_register(const plugin_t* plugin) {
    if (plugin == NULL) {
        fprintf(stderr, "Plugin error: NULL plugin provided\n");
        return -1;
    }
    
    if (plugin_count >= MAX_PLUGINS) {
        fprintf(stderr, "Plugin error: Maximum plugins (%d) exceeded\n", MAX_PLUGINS);
        return -1;
    }
    
    // Validate plugin structure
    if (plugin->name == NULL || plugin->type < 1 || plugin->type > 3) {
        fprintf(stderr, "Plugin error: Invalid plugin structure\n");
        return -1;
    }
    
    registered_plugins[plugin_count] = plugin;
    plugin_count++;
    
    plugin_log("Plugin registered successfully");
    return 0;
}

/**
 * Initialize all registered plugins
 */
int plugin_load_all(void) {
    int failed_count = 0;
    
    for (int i = 0; i < plugin_count; i++) {
        const plugin_t* plugin = registered_plugins[i];
        
        if (plugin->init != NULL) {
            if (plugin->init() != 0) {
                fprintf(stderr, "Plugin initialization failed: %s\n", plugin->name);
                failed_count++;
            }
        }
    }
    
    if (failed_count == 0) {
        plugin_log("All plugins initialized successfully");
    }
    
    return failed_count;
}

/**
 * Cleanup all registered plugins
 */
void plugin_cleanup_all(void) {
    for (int i = 0; i < plugin_count; i++) {
        const plugin_t* plugin = registered_plugins[i];
        
        if (plugin->cleanup != NULL) {
            plugin->cleanup();
        }
    }
    
    plugin_count = 0;
    memset(registered_plugins, 0, sizeof(registered_plugins));
}

/**
 * Transform output through all output transformer plugins
 */
const char* plugin_transform_output(const char* original) {
    if (original == NULL) {
        return NULL;
    }
    
    // Start with original output
    strncpy(output_buffer, original, sizeof(output_buffer) - 1);
    output_buffer[sizeof(output_buffer) - 1] = '\0';
    
    // Apply all output transformer plugins
    for (int i = 0; i < plugin_count; i++) {
        const plugin_t* plugin = registered_plugins[i];
        
        if (plugin->type == PLUGIN_TYPE_OUTPUT_TRANSFORMER && plugin->transform_output != NULL) {
            const char* transformed = plugin->transform_output(output_buffer);
            if (transformed != NULL) {
                strncpy(output_buffer, transformed, sizeof(output_buffer) - 1);
                output_buffer[sizeof(output_buffer) - 1] = '\0';
            }
        }
    }
    
    return output_buffer;
}

/**
 * Run all pre-processor plugins
 */
int plugin_run_pre_processors(void) {
    int error_count = 0;
    
    for (int i = 0; i < plugin_count; i++) {
        const plugin_t* plugin = registered_plugins[i];
        
        if (plugin->type == PLUGIN_TYPE_PRE_PROCESSOR && plugin->process != NULL) {
            if (plugin->process() != 0) {
                error_count++;
            }
        }
    }
    
    return error_count;
}

/**
 * Run all post-processor plugins
 */
int plugin_run_post_processors(void) {
    int error_count = 0;
    
    for (int i = 0; i < plugin_count; i++) {
        const plugin_t* plugin = registered_plugins[i];
        
        if (plugin->type == PLUGIN_TYPE_POST_PROCESSOR && plugin->process != NULL) {
            if (plugin->process() != 0) {
                error_count++;
            }
        }
    }
    
    return error_count;
}

/**
 * Utility logging function for plugins
 */
void plugin_log(const char* message) {
    // Only log in debug mode or when explicitly enabled
    // This prevents plugin messages from interfering with expected output
    const char* debug = getenv("PLUGIN_DEBUG");
    if (debug != NULL && strcmp(debug, "1") == 0) {
        fprintf(stderr, "Plugin: %s\n", message);
    }
}