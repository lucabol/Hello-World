#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../plugin.h"

// Plugin implementation functions
static int emoji_init(void) {
    return 0; // Success
}

static char* emoji_transform(const char* input) {
    if (!input) return NULL;
    
    // Add emoji prefix and suffix
    const char* prefix = "🌍 ";
    const char* suffix = " 🎉";
    
    size_t total_len = strlen(prefix) + strlen(input) + strlen(suffix) + 1;
    char* result = malloc(total_len);
    if (!result) return NULL;
    
    snprintf(result, total_len, "%s%s%s", prefix, input, suffix);
    
    return result;
}

static void emoji_cleanup(void) {
    // Nothing to cleanup for this simple plugin
}

static int emoji_version(void) {
    return PLUGIN_API_VERSION;
}

// Plugin descriptor
static plugin_t emoji_plugin = {
    .name = "emoji",
    .description = "Adds emoji decorations to message",
    .init = emoji_init,
    .transform = emoji_transform,
    .cleanup = emoji_cleanup,
    .version = emoji_version
};

// Required plugin entry point (exported symbol)
__attribute__((visibility("default")))
plugin_t* get_plugin(void) {
    return &emoji_plugin;
}