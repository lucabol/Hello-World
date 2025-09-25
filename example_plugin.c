#include "plugin.h"
#include <string.h>
#include <stdio.h>

/**
 * Example Custom Plugin: Reverse Text Transformer
 * 
 * This plugin demonstrates how external developers can create
 * their own plugins to extend hello.c functionality.
 * 
 * Functionality: Reverses the output text
 */

static char reverse_buffer[1024];

static int reverse_init(void) {
    plugin_log("Reverse text transformer initialized");
    return 0;
}

static const char* reverse_transform(const char* original) {
    if (original == NULL) return NULL;
    
    int len = strlen(original);
    if (len >= (int)sizeof(reverse_buffer)) {
        return original; // Input too long, return unchanged
    }
    
    // Reverse the string
    for (int i = 0; i < len; i++) {
        reverse_buffer[i] = original[len - 1 - i];
    }
    reverse_buffer[len] = '\0';
    
    return reverse_buffer;
}

static void reverse_cleanup(void) {
    plugin_log("Reverse text transformer cleaned up");
}

// Plugin descriptor
static const plugin_t reverse_plugin = {
    .name = "Reverse Text Transformer",
    .version = "1.0.0",
    .author = "External Developer Example",
    .type = PLUGIN_TYPE_OUTPUT_TRANSFORMER,
    .init = reverse_init,
    .cleanup = reverse_cleanup,
    .transform_output = reverse_transform,
    .process = NULL
};

// Registration function
int register_reverse_plugin(void) {
    return plugin_register(&reverse_plugin);
}

/*
 * To use this plugin:
 * 
 * 1. Save as example_plugin.c
 * 2. Create example_plugin.h with:
 *    int register_reverse_plugin(void);
 * 3. Modify hello.c to include example_plugin.h and call register_reverse_plugin()
 * 4. Update Makefile to compile with example_plugin.c
 * 5. Build with: gcc -DENABLE_PLUGINS -o hello hello.c plugin.c example_plugin.c
 * 
 * Result: "Hello world!" becomes "!dlrow olleH"
 */