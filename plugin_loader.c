/* Plugin Loader Implementation
 * 
 * Implements dynamic loading and management of plugins for the Hello World program.
 * Uses dlopen/dlsym on POSIX systems for dynamic library loading.
 */

/* Feature test macros for POSIX functions */
#define _POSIX_C_SOURCE 200809L

#include "plugin_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #define dlopen(path, flags) LoadLibrary(path)
    #define dlsym(handle, name) GetProcAddress((HMODULE)(handle), name)
    #define dlclose(handle) FreeLibrary((HMODULE)(handle))
    #define dlerror() "Windows DLL error"
#else
    #include <dlfcn.h>
#endif

/* Structure to hold plugin information */
typedef struct {
    void* handle;                           /* Handle to the loaded shared library */
    plugin_init_func init_func;             /* Pointer to plugin init function */
    plugin_transform_func transform_func;   /* Pointer to plugin transform function */
    char* name;                             /* Plugin name (filename) */
} plugin_t;

/* Static array to store loaded plugins */
static plugin_t plugins[MAX_PLUGINS];
static int plugin_count = 0;
static int initialized = 0;

/* Initialize the plugin loader system */
int plugin_loader_init(void) {
    if (initialized) {
        return 0;  /* Already initialized */
    }
    
    /* Initialize plugin array */
    for (int i = 0; i < MAX_PLUGINS; i++) {
        plugins[i].handle = NULL;
        plugins[i].init_func = NULL;
        plugins[i].transform_func = NULL;
        plugins[i].name = NULL;
    }
    
    plugin_count = 0;
    initialized = 1;
    return 0;
}

/* Load a plugin from a shared library file */
int plugin_loader_load(const char* plugin_path) {
    void* func_ptr;
    
    if (!initialized) {
        fprintf(stderr, "Plugin loader not initialized\n");
        return -1;
    }
    
    if (plugin_count >= MAX_PLUGINS) {
        fprintf(stderr, "Maximum number of plugins reached\n");
        return -1;
    }
    
    if (plugin_path == NULL) {
        fprintf(stderr, "Plugin path is NULL\n");
        return -1;
    }
    
    /* Load the shared library */
    void* handle = dlopen(plugin_path, RTLD_LAZY);
    if (handle == NULL) {
        fprintf(stderr, "Failed to load plugin '%s': %s\n", plugin_path, dlerror());
        return -1;
    }
    
    /* Clear any existing error */
    dlerror();
    
    /* Load the init function - using intermediate void* to avoid pedantic warning */
    func_ptr = dlsym(handle, PLUGIN_INIT_FUNC);
    const char* error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "Failed to find %s in plugin '%s': %s\n", 
                PLUGIN_INIT_FUNC, plugin_path, error);
        dlclose(handle);
        return -1;
    }
    plugin_init_func init_func;
    memcpy(&init_func, &func_ptr, sizeof(func_ptr));
    
    /* Load the transform function */
    func_ptr = dlsym(handle, PLUGIN_TRANSFORM_FUNC);
    error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "Failed to find %s in plugin '%s': %s\n", 
                PLUGIN_TRANSFORM_FUNC, plugin_path, error);
        dlclose(handle);
        return -1;
    }
    plugin_transform_func transform_func;
    memcpy(&transform_func, &func_ptr, sizeof(func_ptr));
    
    /* Initialize the plugin */
    if (init_func() != 0) {
        fprintf(stderr, "Plugin initialization failed for '%s'\n", plugin_path);
        dlclose(handle);
        return -1;
    }
    
    /* Store plugin information */
    plugins[plugin_count].handle = handle;
    plugins[plugin_count].init_func = init_func;
    plugins[plugin_count].transform_func = transform_func;
    plugins[plugin_count].name = strdup(plugin_path);
    plugin_count++;
    
    return 0;
}

/* Apply all loaded plugins to transform a message */
char* plugin_loader_apply_transforms(const char* input) {
    if (!initialized) {
        return NULL;
    }
    
    if (input == NULL) {
        return NULL;
    }
    
    /* If no plugins are loaded, return a copy of the input */
    if (plugin_count == 0) {
        return strdup(input);
    }
    
    /* Apply plugins in sequence */
    char* current = strdup(input);
    if (current == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < plugin_count; i++) {
        char* transformed = plugins[i].transform_func(current);
        free(current);
        
        if (transformed == NULL) {
            fprintf(stderr, "Plugin '%s' transform failed\n", plugins[i].name);
            return NULL;
        }
        
        current = transformed;
    }
    
    return current;
}

/* Get the number of currently loaded plugins */
int plugin_loader_get_count(void) {
    return plugin_count;
}

/* Cleanup and unload all plugins */
void plugin_loader_cleanup(void) {
    if (!initialized) {
        return;
    }
    
    /* Unload all plugins in reverse order */
    for (int i = plugin_count - 1; i >= 0; i--) {
        if (plugins[i].handle != NULL) {
            dlclose(plugins[i].handle);
            plugins[i].handle = NULL;
        }
        if (plugins[i].name != NULL) {
            free(plugins[i].name);
            plugins[i].name = NULL;
        }
        plugins[i].init_func = NULL;
        plugins[i].transform_func = NULL;
    }
    
    plugin_count = 0;
    initialized = 0;
}
