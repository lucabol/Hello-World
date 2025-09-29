#include <stdio.h>
#include <stdlib.h>
#include "plugin.h"

int main() {
    const char* original_message = "Hello world!";
    char* final_message;
    const char* plugins_dir;
    
    // Allow overriding plugins directory via environment variable
    plugins_dir = getenv("PLUGINS_DIR");
    if (!plugins_dir) {
        plugins_dir = "plugins";
    }
    
    // Load plugins from the specified directory (if it exists)
    load_plugins(plugins_dir);
    
    // Apply plugin transformations
    final_message = apply_plugins(original_message);
    
    // Print the final message (plugins may have transformed it)
    if (final_message) {
        printf("%s", final_message);
        free(final_message);
    } else {
        // Fallback to original message if plugin processing failed
        printf("%s", original_message);
    }
    
    // Cleanup plugins
    cleanup_plugins();
    
    return 0;
}
