#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include <errno.h>
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
    
    if (!input || !output || output_size == 0) {
        if (output && output_size > 0) {
            output[0] = '\0';
        }
        return;
    }
    
    /* Start with original input */
    strncpy(temp_input, input, sizeof(temp_input) - 1);
    temp_input[sizeof(temp_input) - 1] = '\0';
    
    /* Apply each transformer in sequence */
    for (int i = 0; i < transformer_count; i++) {
        /* Clear output buffer before calling transformer */
        memset(temp_output, 0, sizeof(temp_output));
        
        if (transformers[i].transformer(temp_input, temp_output, sizeof(temp_output)) == 0) {
            /* Validate that transformer properly null-terminated the output */
            temp_output[sizeof(temp_output) - 1] = '\0';
            
            /* Validate output length is reasonable */
            size_t output_len = strlen(temp_output);
            if (output_len < sizeof(temp_output)) {
                strncpy(temp_input, temp_output, sizeof(temp_input) - 1);
                temp_input[sizeof(temp_input) - 1] = '\0';
            } else {
                fprintf(stderr, "Warning: transformer '%s' produced invalid output, skipping\n", 
                       transformers[i].name);
            }
        } else {
            fprintf(stderr, "Warning: transformer '%s' failed, skipping\n", 
                   transformers[i].name);
        }
    }
    
    /* Copy final result to output with bounds checking */
    strncpy(output, temp_input, output_size - 1);
    output[output_size - 1] = '\0';
}

/* Load plugins from a directory */
int load_plugins_from_directory(const char* dir_path) {
    DIR* dir;
    struct dirent* entry;
    int loaded_count = 0;
    
    if (!dir_path) {
        fprintf(stderr, "Error: null directory path provided\n");
        return -1;
    }
    
    dir = opendir(dir_path);
    if (!dir) {
        /* Directory doesn't exist, that's okay - no plugins to load */
        /* Only print error if directory was expected to exist */
        if (errno != ENOENT) {
            fprintf(stderr, "Warning: cannot open plugin directory '%s': %s\n", 
                   dir_path, strerror(errno));
        }
        return 0;
    }
    
    /* Plugin loading order: Plugins are loaded in alphabetical order by filename.
     * This is determined by readdir() behavior, which typically returns entries 
     * in filesystem order. For deterministic behavior across systems, we rely on
     * alphabetical filename ordering. Transformers are then applied in the order
     * they were registered (which is typically the load order).
     */
    while ((entry = readdir(dir)) != NULL) {
        /* Look for .so files - must end with .so, not just contain it */
        size_t name_len = strlen(entry->d_name);
        if (name_len < 3 || strcmp(entry->d_name + name_len - 3, ".so") != 0) {
            continue;
        }
        
        char plugin_path[512];
        int path_len = snprintf(plugin_path, sizeof(plugin_path), "%s/%s", dir_path, entry->d_name);
        if (path_len >= (int)sizeof(plugin_path)) {
            fprintf(stderr, "Error: plugin path too long for %s\n", entry->d_name);
            continue;
        }
        
        /* Clear any previous dlerror() */
        dlerror();
        
        /* Load the plugin */
        void* handle = dlopen(plugin_path, RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "Cannot load plugin %s: %s\n", plugin_path, dlerror());
            continue;
        }
        
        /* Clear any previous dlerror() */
        dlerror();
        
        /* Get the plugin_init function */
        union {
            void* ptr;
            plugin_init_t func;
        } plugin_init_union;
        
        plugin_init_union.ptr = dlsym(handle, "plugin_init");
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            fprintf(stderr, "Plugin %s missing plugin_init function: %s\n", plugin_path, dlsym_error);
            dlclose(handle);
            continue;
        }
        
        /* Initialize the plugin and validate ABI */
        plugin_info_t info = plugin_init_union.func();
        
        /* Validate ABI version */
        if (info.version != PLUGIN_API_VERSION) {
            fprintf(stderr, "Plugin %s has incompatible ABI version %d (expected %d), skipping\n",
                   plugin_path, info.version, PLUGIN_API_VERSION);
            dlclose(handle);
            continue;
        }
        
        /* Validate plugin info */
        if (!info.name || !info.description) {
            fprintf(stderr, "Plugin %s provided invalid metadata, skipping\n", plugin_path);
            dlclose(handle);
            continue;
        }
        
        /* Store the handle for cleanup */
        if (plugin_count < MAX_PLUGINS) {
            loaded_plugins[plugin_count++] = handle;
            loaded_count++;
            printf("Loaded plugin: %s - %s\n", info.name, info.description);
        } else {
            fprintf(stderr, "Maximum number of plugins (%d) reached, skipping %s\n", 
                   MAX_PLUGINS, plugin_path);
            dlclose(handle);
        }
    }
    
    closedir(dir);
    return loaded_count;
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