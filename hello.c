#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include "plugin.h"

#define MAX_PLUGINS 32
#define PLUGINS_DIR "./plugins"

/* Plugin manager structure */
typedef struct {
    void* handle;
    plugin_interface_t* interface;
} loaded_plugin_t;

/* Global plugin storage */
static loaded_plugin_t plugins[MAX_PLUGINS];
static int plugin_count = 0;

/* Load a single plugin */
static int load_plugin(const char* filename) {
    if (plugin_count >= MAX_PLUGINS) {
        fprintf(stderr, "Warning: Maximum number of plugins reached\n");
        return 0;
    }
    
    void* handle = dlopen(filename, RTLD_LAZY);
    if (!handle) {
        /* Silently ignore plugin loading errors for optional functionality */
        return 0;
    }
    
    /* Get the plugin interface */
    plugin_interface_t* (*get_interface)(void) = dlsym(handle, "get_plugin_interface");
    if (!get_interface) {
        dlclose(handle);
        return 0;
    }
    
    plugin_interface_t* interface = get_interface();
    if (!interface || !interface->get_info) {
        dlclose(handle);
        return 0;
    }
    
    /* Check API version compatibility */
    plugin_info_t* info = interface->get_info();
    if (!info || info->api_version != PLUGIN_API_VERSION) {
        dlclose(handle);
        return 0;
    }
    
    /* Initialize plugin if it has an init function */
    if (interface->init && interface->init() != 0) {
        dlclose(handle);
        return 0;
    }
    
    /* Store the plugin */
    plugins[plugin_count].handle = handle;
    plugins[plugin_count].interface = interface;
    plugin_count++;
    
    return 1;
}

/* Load all plugins from the plugins directory */
static void load_plugins(void) {
    DIR* dir = opendir(PLUGINS_DIR);
    if (!dir) {
        /* No plugins directory - continue without plugins */
        return;
    }
    
    struct dirent* entry;
    char filepath[512];
    
    while ((entry = readdir(dir)) != NULL) {
        /* Look for .so files (shared libraries) */
        if (strstr(entry->d_name, ".so")) {
            snprintf(filepath, sizeof(filepath), "%s/%s", PLUGINS_DIR, entry->d_name);
            load_plugin(filepath);
        }
    }
    
    closedir(dir);
}

/* Apply message transformers to the input message */
static char* apply_transformers(const char* message) {
    char* current_message = strdup(message);
    if (!current_message) return NULL;
    
    for (int i = 0; i < plugin_count; i++) {
        plugin_interface_t* interface = plugins[i].interface;
        plugin_info_t* info = interface->get_info();
        
        if (info->type == PLUGIN_TYPE_MESSAGE_TRANSFORMER && interface->transform_message) {
            char* new_message = interface->transform_message(current_message);
            if (new_message) {
                free(current_message);
                current_message = new_message;
            }
        }
    }
    
    return current_message;
}

/* Apply output formatters to display the message */
static void apply_formatters(const char* message) {
    int formatted = 0;
    
    for (int i = 0; i < plugin_count; i++) {
        plugin_interface_t* interface = plugins[i].interface;
        plugin_info_t* info = interface->get_info();
        
        if (info->type == PLUGIN_TYPE_OUTPUT_FORMATTER && interface->format_output) {
            interface->format_output(message);
            formatted = 1;
        }
    }
    
    /* If no formatters handled the output, use default behavior */
    if (!formatted) {
        printf("%s", message);
    }
}

/* Cleanup all loaded plugins */
static void cleanup_plugins(void) {
    for (int i = 0; i < plugin_count; i++) {
        if (plugins[i].interface->cleanup) {
            plugins[i].interface->cleanup();
        }
        dlclose(plugins[i].handle);
    }
}

int main() {
    const char* base_message = "Hello world!";
    
    /* Load plugins */
    load_plugins();
    
    /* Transform the message through plugins */
    char* transformed_message = apply_transformers(base_message);
    if (!transformed_message) {
        transformed_message = strdup(base_message);
    }
    
    /* Output the message through formatters or default */
    apply_formatters(transformed_message);
    
    /* Cleanup */
    free(transformed_message);
    cleanup_plugins();
    
    return 0;
}
