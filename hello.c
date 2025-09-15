#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef PLUGIN_ENABLED
#include <dlfcn.h>
#include "plugin.h"
#endif

int main(){
    const char* message = "Hello world!";
    char* final_message = NULL;
    
#ifdef PLUGIN_ENABLED
    /* Load plugin if specified via environment variable */
    const char* plugin_path = getenv("HELLO_PLUGIN");
    if (plugin_path) {
        void* handle = dlopen(plugin_path, RTLD_LAZY);
        if (handle) {
            plugin_t* (*register_func)(void) = (plugin_t* (*)(void))dlsym(handle, "plugin_register");
            if (register_func) {
                plugin_t* plugin = register_func();
                if (plugin) {
                    if (plugin->init) plugin->init();
                    if (plugin->transform) {
                        final_message = plugin->transform(message);
                        if (final_message) message = final_message;
                    }
                }
            }
            /* Note: We don't call cleanup or dlclose for simplicity */
        }
    }
#endif
    
    printf("%s", message);
    
    if (final_message) free(final_message);
    return 0;
}
