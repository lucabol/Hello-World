#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "plugin.h"

// Sample plugins demonstrating the plugin system

/* ========================================
 * Plugin 1: Uppercase Transformer
 * Transforms output to uppercase
 * ======================================== */

static char uppercase_buffer[1024];

static int uppercase_init(void) {
    plugin_log("Uppercase transformer initialized");
    return 0;
}

static const char* uppercase_transform(const char* original) {
    if (original == NULL) return NULL;
    
    strncpy(uppercase_buffer, original, sizeof(uppercase_buffer) - 1);
    uppercase_buffer[sizeof(uppercase_buffer) - 1] = '\0';
    
    for (int i = 0; uppercase_buffer[i]; i++) {
        if (uppercase_buffer[i] >= 'a' && uppercase_buffer[i] <= 'z') {
            uppercase_buffer[i] = uppercase_buffer[i] - 'a' + 'A';
        }
    }
    
    return uppercase_buffer;
}

static void uppercase_cleanup(void) {
    plugin_log("Uppercase transformer cleaned up");
}

static const plugin_t uppercase_plugin = {
    .name = "Uppercase Transformer",
    .version = "1.0.0",
    .author = "Plugin System Demo",
    .type = PLUGIN_TYPE_OUTPUT_TRANSFORMER,
    .init = uppercase_init,
    .cleanup = uppercase_cleanup,
    .transform_output = uppercase_transform,
    .process = NULL
};

/* ========================================
 * Plugin 2: Greeting Pre-processor
 * Displays a greeting before main execution
 * ======================================== */

static int greeting_init(void) {
    plugin_log("Greeting pre-processor initialized");
    return 0;
}

static int greeting_process(void) {
    printf("*** Plugin System Active ***\n");
    return 0;
}

static const plugin_t greeting_plugin = {
    .name = "Greeting Pre-processor",
    .version = "1.0.0", 
    .author = "Plugin System Demo",
    .type = PLUGIN_TYPE_PRE_PROCESSOR,
    .init = greeting_init,
    .cleanup = NULL,
    .transform_output = NULL,
    .process = greeting_process
};

/* ========================================
 * Plugin 3: Statistics Post-processor
 * Shows statistics after main execution
 * ======================================== */

static int stats_init(void) {
    plugin_log("Statistics post-processor initialized");
    return 0;
}

static int stats_process(void) {
    printf("*** Plugin execution complete ***\n");
    return 0;
}

static const plugin_t stats_plugin = {
    .name = "Statistics Post-processor",
    .version = "1.0.0",
    .author = "Plugin System Demo", 
    .type = PLUGIN_TYPE_POST_PROCESSOR,
    .init = stats_init,
    .cleanup = NULL,
    .transform_output = NULL,
    .process = stats_process
};

/* ========================================
 * Plugin Registration Function
 * Call this to register all sample plugins
 * ======================================== */

int register_sample_plugins(void) {
    int result = 0;
    
    if (plugin_register(&uppercase_plugin) != 0) {
        result = -1;
    }
    
    if (plugin_register(&greeting_plugin) != 0) {
        result = -1;
    }
    
    if (plugin_register(&stats_plugin) != 0) {
        result = -1;
    }
    
    return result;
}

// Function to register only the uppercase plugin (for backward compatibility tests)
int register_uppercase_plugin_only(void) {
    return plugin_register(&uppercase_plugin);
}