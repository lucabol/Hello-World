# include <stdio.h>
#include <stdlib.h>

#ifndef UNIT_TEST
/* Plugin support - only in non-test builds */
#include <dlfcn.h>
#include "plugin.h"

/* Load and apply plugin transformation if HELLO_PLUGIN is set */
static const char* apply_plugin(const char* message) {
    const char* plugin_path = getenv("HELLO_PLUGIN");
    if (!plugin_path) {
        return message; /* No plugin, return original */
    }
    
    /* Load plugin library */
    void* plugin_handle = dlopen(plugin_path, RTLD_LAZY);
    if (!plugin_handle) {
        fprintf(stderr, "Warning: Failed to load plugin: %s\n", dlerror());
        return message;
    }
    
    /* Get plugin transform function (using union to avoid pedantic warning) */
    plugin_transform_func transform;
    union {
        void* obj;
        plugin_transform_func func;
    } alias;
    alias.obj = dlsym(plugin_handle, PLUGIN_TRANSFORM_NAME);
    transform = alias.func;
    
    if (!transform) {
        fprintf(stderr, "Warning: Plugin missing %s function\n", PLUGIN_TRANSFORM_NAME);
        dlclose(plugin_handle);
        return message;
    }
    
    /* Apply transformation */
    const char* result = transform(message);
    
    /* Note: We intentionally don't close plugin_handle here as the transformed
     * message might reference plugin memory. In a production system, you'd want
     * proper lifecycle management with cleanup functions. */
    
    return result ? result : message;
}
#endif /* UNIT_TEST */

/* Get greeting message - used by unit tests */
const char* get_greeting(void) {
    return "Hello world!";
}

#ifndef UNIT_TEST
int main(){
    const char* message = get_greeting();
    
    /* Apply plugin transformation if configured */
    message = apply_plugin(message);
    
    printf("%s", message);
}
#endif /* UNIT_TEST */
