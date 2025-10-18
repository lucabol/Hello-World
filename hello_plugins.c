/* Plugin System Complete Demo
 * 
 * Demonstrates the plugin system with multiple plugins and scenarios.
 * Shows how plugins can be chained together to create complex transformations.
 */
#include <stdio.h>
#include "plugin.h"

/* Original greeting message */
static const char* get_greeting(void) {
    return "Hello world!";
}

/* External plugin declarations */
extern plugin_info_t plugin_info;  /* Will be defined by linked plugin */

int main(int argc, char* argv[]) {
    char output[256];
    int demo_mode = 0;
    
    /* Check for demo mode argument */
    if (argc > 1) {
        demo_mode = 1;
    }
    
    /* Initialize the plugin system */
    plugin_system_init();
    
    if (demo_mode) {
        printf("=== Plugin System Demo ===\n\n");
        
        /* Demo 1: No plugins */
        printf("1. Original (no plugins):\n");
        plugin_apply_all(get_greeting(), output, sizeof(output));
        printf("   Output: %s\n\n", output);
        
        /* Demo 2: With single plugin */
        printf("2. With single plugin:\n");
        plugin_register(&plugin_info);
        plugin_apply_all(get_greeting(), output, sizeof(output));
        printf("   Output: %s\n\n", output);
        
        printf("Plugin system demonstration complete!\n");
    } else {
        /* Non-demo mode: just apply plugins and output result */
        plugin_register(&plugin_info);
        plugin_apply_all(get_greeting(), output, sizeof(output));
        printf("%s\n", output);
    }
    
    /* Cleanup */
    plugin_system_cleanup();
    
    return 0;
}
