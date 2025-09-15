#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../plugin.h"

static void uppercase_init(void) {
    /* Plugin initialization - could load config, etc. */
}

static char* uppercase_transform(const char* input) {
    if (!input) return NULL;
    
    size_t len = strlen(input);
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = toupper(input[i]);
    }
    result[len] = '\0';
    
    return result;
}

static void uppercase_cleanup(void) {
    /* Plugin cleanup */
}

static plugin_t uppercase_plugin = {
    .name = "uppercase",
    .version = "1.0.0",
    .init = uppercase_init,
    .transform = uppercase_transform,
    .cleanup = uppercase_cleanup
};

plugin_t* plugin_register(void) {
    return &uppercase_plugin;
}