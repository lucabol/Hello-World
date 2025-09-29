#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/stat.h>
#include "plugin.h"

// Maximum number of plugins
#define MAX_PLUGINS 16

// Global plugin state
static plugin_t plugins[MAX_PLUGINS];
static void* plugin_handles[MAX_PLUGINS];
static int plugin_count = 0;

int load_plugins(const char* plugin_dir) {
    DIR* dir;
    struct dirent* entry;
    char plugin_path[512];
    void* handle;
    plugin_t* (*get_plugin)(void);
    plugin_t* plugin;
    
    if (!plugin_dir) {
        return 0; // No plugins directory specified
    }
    
    dir = opendir(plugin_dir);
    if (!dir) {
        return 0; // Directory doesn't exist, which is fine
    }
    
    while ((entry = readdir(dir)) != NULL && plugin_count < MAX_PLUGINS) {
        // Look for .so files (shared libraries)
        if (strstr(entry->d_name, ".so") == NULL) {
            continue;
        }
        
        snprintf(plugin_path, sizeof(plugin_path), "%s/%s", plugin_dir, entry->d_name);
        
        // Load the plugin
        handle = dlopen(plugin_path, RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "Warning: Cannot load plugin %s: %s\n", plugin_path, dlerror());
            continue;
        }
        
        // Get the plugin descriptor with proper casting for pedantic compliance
        *(void**)(&get_plugin) = dlsym(handle, "get_plugin");
        if (!get_plugin) {
            fprintf(stderr, "Warning: Plugin %s missing get_plugin function\n", plugin_path);
            dlclose(handle);
            continue;
        }
        
        plugin = get_plugin();
        if (!plugin || !plugin->transform) {
            fprintf(stderr, "Warning: Plugin %s has invalid descriptor\n", plugin_path);
            dlclose(handle);
            continue;
        }
        
        // Check API version compatibility
        if (plugin->version && plugin->version() != PLUGIN_API_VERSION) {
            fprintf(stderr, "Warning: Plugin %s has incompatible API version\n", plugin_path);
            dlclose(handle);
            continue;
        }
        
        // Initialize the plugin
        if (plugin->init && plugin->init() != 0) {
            fprintf(stderr, "Warning: Plugin %s initialization failed\n", plugin_path);
            dlclose(handle);
            continue;
        }
        
        // Store the plugin
        plugins[plugin_count] = *plugin;
        plugin_handles[plugin_count] = handle;
        plugin_count++;
    }
    
    closedir(dir);
    return plugin_count;
}

char* apply_plugins(const char* message) {
    char* current_message;
    char* new_message;
    int i;
    
    if (!message || plugin_count == 0) {
        // No plugins or no message, return a copy of the original
        if (!message) return NULL;
        current_message = malloc(strlen(message) + 1);
        if (current_message) {
            strcpy(current_message, message);
        }
        return current_message;
    }
    
    // Start with a copy of the original message
    current_message = malloc(strlen(message) + 1);
    if (!current_message) {
        return NULL;
    }
    strcpy(current_message, message);
    
    // Apply each plugin transformation
    for (i = 0; i < plugin_count; i++) {
        if (plugins[i].transform) {
            new_message = plugins[i].transform(current_message);
            if (new_message && new_message != current_message) {
                free(current_message);
                current_message = new_message;
            }
        }
    }
    
    return current_message;
}

void cleanup_plugins(void) {
    int i;
    
    for (i = 0; i < plugin_count; i++) {
        if (plugins[i].cleanup) {
            plugins[i].cleanup();
        }
        if (plugin_handles[i]) {
            dlclose(plugin_handles[i]);
        }
    }
    
    plugin_count = 0;
}