#include "plugin_loader.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

/* Maximum number of plugins */
#define MAX_PLUGINS 32

/* Plugin registry */
static plugin_t plugins[MAX_PLUGINS];
static int plugin_count = 0;

/* Plugin directory path */
static const char* PLUGIN_DIR = "./plugins";

/* Helper function to duplicate string */
static char* duplicate_string(const char* str) {
    size_t len = strlen(str);
    char* dup = malloc(len + 1);
    if (dup) {
        memcpy(dup, str, len + 1);
    }
    return dup;
}

/* Helper to convert dlsym result to function pointer */
static plugin_get_info_func get_function_pointer_get_info(void* handle, const char* symbol) {
    void* ptr = dlsym(handle, symbol);
    plugin_get_info_func func;
    memcpy(&func, &ptr, sizeof(func));
    return func;
}

static plugin_init_func get_function_pointer_init(void* handle, const char* symbol) {
    void* ptr = dlsym(handle, symbol);
    plugin_init_func func;
    memcpy(&func, &ptr, sizeof(func));
    return func;
}

static plugin_filter_func get_function_pointer_filter(void* handle, const char* symbol) {
    void* ptr = dlsym(handle, symbol);
    plugin_filter_func func;
    memcpy(&func, &ptr, sizeof(func));
    return func;
}

static plugin_process_func get_function_pointer_process(void* handle, const char* symbol) {
    void* ptr = dlsym(handle, symbol);
    plugin_process_func func;
    memcpy(&func, &ptr, sizeof(func));
    return func;
}

static plugin_cleanup_func get_function_pointer_cleanup(void* handle, const char* symbol) {
    void* ptr = dlsym(handle, symbol);
    plugin_cleanup_func func;
    memcpy(&func, &ptr, sizeof(func));
    return func;
}

static int is_shared_library(const char* filename) {
    size_t len = strlen(filename);
    return (len > 3 && strcmp(filename + len - 3, ".so") == 0);
}

static int load_plugin(const char* filepath) {
    if (plugin_count >= MAX_PLUGINS) {
        fprintf(stderr, "Plugin loader: Maximum number of plugins reached\n");
        return -1;
    }
    
    plugin_t* plugin = &plugins[plugin_count];
    
    /* Load the shared library */
    plugin->handle = dlopen(filepath, RTLD_LAZY);
    if (!plugin->handle) {
        fprintf(stderr, "Plugin loader: Cannot load %s: %s\n", filepath, dlerror());
        return -1;
    }
    
    /* Get plugin info function */
    plugin->get_info = get_function_pointer_get_info(plugin->handle, PLUGIN_GET_INFO_FUNC);
    if (!plugin->get_info) {
        fprintf(stderr, "Plugin loader: %s missing %s function\n", filepath, PLUGIN_GET_INFO_FUNC);
        dlclose(plugin->handle);
        return -1;
    }
    
    /* Get plugin information */
    plugin->info = plugin->get_info();
    if (!plugin->info) {
        fprintf(stderr, "Plugin loader: %s get_info returned NULL\n", filepath);
        dlclose(plugin->handle);
        return -1;
    }
    
    /* Check API version compatibility */
    if (plugin->info->api_version != PLUGIN_API_VERSION) {
        fprintf(stderr, "Plugin loader: %s API version mismatch (expected %d, got %d)\n", 
                filepath, PLUGIN_API_VERSION, plugin->info->api_version);
        dlclose(plugin->handle);
        return -1;
    }
    
    /* Load optional functions based on plugin type */
    plugin->init = get_function_pointer_init(plugin->handle, PLUGIN_INIT_FUNC);
    plugin->cleanup = get_function_pointer_cleanup(plugin->handle, PLUGIN_CLEANUP_FUNC);
    
    switch (plugin->info->type) {
        case PLUGIN_TYPE_OUTPUT_FILTER:
            plugin->filter = get_function_pointer_filter(plugin->handle, PLUGIN_FILTER_FUNC);
            if (!plugin->filter) {
                fprintf(stderr, "Plugin loader: Filter plugin %s missing %s function\n", 
                        filepath, PLUGIN_FILTER_FUNC);
                dlclose(plugin->handle);
                return -1;
            }
            break;
            
        case PLUGIN_TYPE_PRE_PROCESSOR:
        case PLUGIN_TYPE_POST_PROCESSOR:
            plugin->process = get_function_pointer_process(plugin->handle, PLUGIN_PROCESS_FUNC);
            if (!plugin->process) {
                fprintf(stderr, "Plugin loader: Processor plugin %s missing %s function\n", 
                        filepath, PLUGIN_PROCESS_FUNC);
                dlclose(plugin->handle);
                return -1;
            }
            break;
            
        default:
            fprintf(stderr, "Plugin loader: %s has unknown plugin type %d\n", 
                    filepath, plugin->info->type);
            dlclose(plugin->handle);
            return -1;
    }
    
    /* Initialize plugin if init function exists */
    if (plugin->init && plugin->init() != 0) {
        fprintf(stderr, "Plugin loader: %s initialization failed\n", filepath);
        dlclose(plugin->handle);
        return -1;
    }
    
    plugin_count++;
    printf("Plugin loader: Loaded %s v%s - %s\n", 
           plugin->info->name, plugin->info->version, plugin->info->description);
    
    return 0;
}

int plugin_loader_init(const char* plugin_dir) {
    DIR* dir;
    struct dirent* entry;
    char filepath[512];
    struct stat statbuf;
    
    const char* search_dir = plugin_dir ? plugin_dir : PLUGIN_DIR;
    
    /* Check if plugin directory exists */
    if (stat(search_dir, &statbuf) != 0 || !S_ISDIR(statbuf.st_mode)) {
        /* Plugin directory doesn't exist - this is not an error, just no plugins to load */
        return 0;
    }
    
    dir = opendir(search_dir);
    if (!dir) {
        /* Cannot open directory - not a fatal error */
        return 0;
    }
    
    /* First pass: count potential plugins to avoid unnecessary output */
    int candidate_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (!is_shared_library(entry->d_name)) {
            continue;
        }
        
        snprintf(filepath, sizeof(filepath), "%s/%s", search_dir, entry->d_name);
        
        if (stat(filepath, &statbuf) != 0 || !S_ISREG(statbuf.st_mode)) {
            continue;
        }
        
        candidate_count++;
    }
    
    /* If no plugins found, return silently */
    if (candidate_count == 0) {
        closedir(dir);
        return 0;
    }
    
    /* Rewind directory for second pass */
    rewinddir(dir);
    
    printf("Plugin loader: Scanning %s for plugins...\n", search_dir);
    
    while ((entry = readdir(dir)) != NULL) {
        if (!is_shared_library(entry->d_name)) {
            continue;
        }
        
        snprintf(filepath, sizeof(filepath), "%s/%s", search_dir, entry->d_name);
        
        if (stat(filepath, &statbuf) != 0 || !S_ISREG(statbuf.st_mode)) {
            continue;
        }
        
        printf("Plugin loader: Found plugin candidate: %s\n", entry->d_name);
        load_plugin(filepath);
    }
    
    closedir(dir);
    
    if (plugin_count > 0) {
        printf("Plugin loader: Successfully loaded %d plugin(s)\n", plugin_count);
    }
    
    return plugin_count;
}

void plugin_loader_cleanup(void) {
    for (int i = 0; i < plugin_count; i++) {
        plugin_t* plugin = &plugins[i];
        
        if (plugin->cleanup) {
            plugin->cleanup();
        }
        
        if (plugin->handle) {
            dlclose(plugin->handle);
        }
    }
    
    plugin_count = 0;
}

char* plugin_apply_filters(const char* input) {
    char* result = NULL;
    char* current_input = (char*)input;
    char* temp_result = NULL;
    
    for (int i = 0; i < plugin_count; i++) {
        plugin_t* plugin = &plugins[i];
        
        if (plugin->info->type == PLUGIN_TYPE_OUTPUT_FILTER && plugin->filter) {
            temp_result = plugin->filter(current_input);
            if (temp_result) {
                /* Free previous result if it was allocated */
                if (result && result != input) {
                    free(result);
                }
                result = temp_result;
                current_input = result;
            }
        }
    }
    
    /* If no filters modified the input, return a copy */
    if (!result) {
        result = duplicate_string(input);
    }
    
    return result;
}

void plugin_run_preprocessors(void) {
    for (int i = 0; i < plugin_count; i++) {
        plugin_t* plugin = &plugins[i];
        
        if (plugin->info->type == PLUGIN_TYPE_PRE_PROCESSOR && plugin->process) {
            plugin->process();
        }
    }
}

void plugin_run_postprocessors(void) {
    for (int i = 0; i < plugin_count; i++) {
        plugin_t* plugin = &plugins[i];
        
        if (plugin->info->type == PLUGIN_TYPE_POST_PROCESSOR && plugin->process) {
            plugin->process();
        }
    }
}

int plugin_get_count(void) {
    return plugin_count;
}

const plugin_info_t* plugin_get_info_by_index(int index) {
    if (index < 0 || index >= plugin_count) {
        return NULL;
    }
    
    return plugins[index].info;
}