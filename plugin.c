#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include "plugin.h"

/* Internal plugin management */
typedef struct {
    char name[64];
    message_transformer_t transformer;
} transformer_entry_t;

static transformer_entry_t transformers[MAX_PLUGINS];
static int transformer_count = 0;
static void* loaded_plugins[MAX_PLUGINS];
static int plugin_count = 0;

/* Register a message transformer */
int register_transformer(const char* name, message_transformer_t transformer) {
    if (transformer_count >= MAX_PLUGINS) {
        fprintf(stderr, "Maximum number of transformers reached\n");
        return -1;
    }
    
    strncpy(transformers[transformer_count].name, name, sizeof(transformers[transformer_count].name) - 1);
    transformers[transformer_count].name[sizeof(transformers[transformer_count].name) - 1] = '\0';
    transformers[transformer_count].transformer = transformer;
    transformer_count++;
    
    return 0;
}

/* Apply all registered transformers to the message */
void apply_transformers(const char* input, char* output, size_t output_size) {
    char temp_input[MAX_MESSAGE_SIZE];
    char temp_output[MAX_MESSAGE_SIZE];
    
    /* Start with original input */
    strncpy(temp_input, input, sizeof(temp_input) - 1);
    temp_input[sizeof(temp_input) - 1] = '\0';
    
    /* Apply each transformer in sequence */
    for (int i = 0; i < transformer_count; i++) {
        if (transformers[i].transformer(temp_input, temp_output, sizeof(temp_output)) == 0) {
            strncpy(temp_input, temp_output, sizeof(temp_input) - 1);
            temp_input[sizeof(temp_input) - 1] = '\0';
        }
    }
    
    /* Copy final result to output */
    strncpy(output, temp_input, output_size - 1);
    output[output_size - 1] = '\0';
}

/* Load plugins from a directory */
int load_plugins_from_directory(const char* dir_path) {
    DIR* dir = opendir(dir_path);
    if (!dir) {
        /* Directory doesn't exist, that's okay - no plugins to load */
        return 0;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        /* Look for .so files */
        if (strstr(entry->d_name, ".so") != NULL) {
            char plugin_path[512];
            snprintf(plugin_path, sizeof(plugin_path), "%s/%s", dir_path, entry->d_name);
            
            /* Load the plugin */
            void* handle = dlopen(plugin_path, RTLD_LAZY);
            if (!handle) {
                fprintf(stderr, "Cannot load plugin %s: %s\n", plugin_path, dlerror());
                continue;
            }
            
            /* Get the plugin_init function */
            union {
                void* ptr;
                plugin_init_t func;
            } plugin_init_union;
            
            plugin_init_union.ptr = dlsym(handle, "plugin_init");
            if (!plugin_init_union.ptr) {
                fprintf(stderr, "Plugin %s missing plugin_init function: %s\n", plugin_path, dlerror());
                dlclose(handle);
                continue;
            }
            
            /* Initialize the plugin */
            plugin_info_t info = plugin_init_union.func();
            (void)info; /* Suppress unused variable warning */
            
            /* Store the handle for cleanup */
            if (plugin_count < MAX_PLUGINS) {
                loaded_plugins[plugin_count++] = handle;
            }
        }
    }
    
    closedir(dir);
    return 0;
}

/* Clean up loaded plugins */
void cleanup_plugins(void) {
    for (int i = 0; i < plugin_count; i++) {
        if (loaded_plugins[i]) {
            dlclose(loaded_plugins[i]);
        }
    }
    plugin_count = 0;
    transformer_count = 0;
}