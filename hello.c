/* Feature test macros for POSIX functions */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hello.h"

#ifdef WITH_PLUGINS
#include "plugin_loader.h"
#endif

/* Get the greeting message */
const char* get_greeting(void) {
    return "Hello world!";
}

#ifndef UNIT_TEST
int main(void) {
    const char* greeting = get_greeting();
    
#ifdef WITH_PLUGINS
    /* Check if plugin loading is enabled via environment variable */
    const char* plugins_env = getenv("HELLO_PLUGINS");
    
    if (plugins_env != NULL && plugins_env[0] != '\0') {
        /* Plugin system is enabled - initialize and load plugins */
        if (plugin_loader_init() == 0) {
            /* Parse and load plugins from comma-separated list */
            char* plugins_copy = strdup(plugins_env);
            if (plugins_copy != NULL) {
                char* token = strtok(plugins_copy, ",");
                while (token != NULL) {
                    plugin_loader_load(token);
                    token = strtok(NULL, ",");
                }
                free(plugins_copy);
            }
            
            /* Apply plugin transformations if any plugins were loaded */
            if (plugin_loader_get_count() > 0) {
                char* transformed = plugin_loader_apply_transforms(greeting);
                if (transformed != NULL) {
                    printf("%s", transformed);
                    free(transformed);
                } else {
                    printf("%s", greeting);
                }
                plugin_loader_cleanup();
            } else {
                /* No plugins loaded, use original greeting */
                printf("%s", greeting);
            }
        } else {
            /* Plugin initialization failed, use original greeting */
            printf("%s", greeting);
        }
    } else {
        /* Plugin system not enabled - use original greeting */
        printf("%s", greeting);
    }
#else
    /* No plugin support compiled in - use original greeting */
    printf("%s", greeting);
#endif
    
    return 0;
}
#endif
