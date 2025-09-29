#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../plugin.h"

// Plugin implementation functions
static int uppercase_init(void) {
    return 0; // Success
}

static char* uppercase_transform(const char* input) {
    if (!input) return NULL;
    
    char* result = malloc(strlen(input) + 1);
    if (!result) return NULL;
    
    int i;
    for (i = 0; input[i]; i++) {
        result[i] = toupper(input[i]);
    }
    result[i] = '\0';
    
    return result;
}

static void uppercase_cleanup(void) {
    // Nothing to cleanup for this simple plugin
}

static int uppercase_version(void) {
    return PLUGIN_API_VERSION;
}

// Plugin descriptor
static plugin_t uppercase_plugin = {
    .name = "uppercase",
    .description = "Converts message to uppercase",
    .init = uppercase_init,
    .transform = uppercase_transform,
    .cleanup = uppercase_cleanup,
    .version = uppercase_version
};

// Required plugin entry point
plugin_t* get_plugin(void) {
    return &uppercase_plugin;
}