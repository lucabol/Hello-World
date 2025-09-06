#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "plugin.h"

/* Plugin registry */
static plugin_t* plugins[10] = {0};
static int plugin_count = 0;

void register_plugin(plugin_t* plugin) {
    if (plugin_count < 10) {
        plugins[plugin_count++] = plugin;
    }
}

void execute_pre_hooks(void) {
    for (int i = 0; i < plugin_count; i++) {
        if (plugins[i]->pre_main_hook) {
            plugins[i]->pre_main_hook();
        }
    }
}

char* apply_transformations(const char* message) {
    char* result = malloc(strlen(message) + 1);
    strcpy(result, message);
    
    for (int i = 0; i < plugin_count; i++) {
        if (plugins[i]->transform_message) {
            char* transformed = plugins[i]->transform_message(result);
            if (transformed != result) {
                free(result);
                result = transformed;
            }
        }
    }
    return result;
}

void execute_post_hooks(void) {
    for (int i = 0; i < plugin_count; i++) {
        if (plugins[i]->post_main_hook) {
            plugins[i]->post_main_hook();
        }
    }
}

int main(){
    execute_pre_hooks();
    
    char* message = apply_transformations("Hello world!");
    printf("%s", message);
    free(message);
    
    execute_post_hooks();
    return 0;
}
