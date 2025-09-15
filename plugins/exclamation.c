#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../plugin.h"

static void exclamation_init(void) {
    /* Plugin initialization */
}

static char* exclamation_transform(const char* input) {
    if (!input) return NULL;
    
    size_t len = strlen(input);
    char* result = malloc(len + 4); /* +3 for "!!!" and +1 for null terminator */
    if (!result) return NULL;
    
    strcpy(result, input);
    strcat(result, "!!!");
    
    return result;
}

static void exclamation_cleanup(void) {
    /* Plugin cleanup */
}

static plugin_t exclamation_plugin = {
    .name = "exclamation",
    .version = "1.0.0", 
    .init = exclamation_init,
    .transform = exclamation_transform,
    .cleanup = exclamation_cleanup
};

plugin_t* plugin_register(void) {
    return &exclamation_plugin;
}