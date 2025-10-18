/* Multi-Plugin Chain Demo
 * 
 * Demonstrates how multiple plugins can be chained together
 * to create complex transformations.
 */
#include <stdio.h>
#include "plugin.h"

/* Original greeting message */
static const char* get_greeting(void) {
    return "Hello world";
}

/* External plugin declarations */
extern plugin_info_t plugin_info;  /* Primary plugin */

int main(void) {
    char output[256];
    
    printf("=== Plugin Chain Demonstration ===\n\n");
    
    /* Initialize the plugin system */
    plugin_system_init();
    
    /* Show original message */
    printf("Original message:\n");
    printf("  \"%s\"\n\n", get_greeting());
    
    /* Apply single plugin */
    printf("After applying plugin:\n");
    plugin_register(&plugin_info);
    plugin_apply_all(get_greeting(), output, sizeof(output));
    printf("  \"%s\"\n\n", output);
    
    printf("Plugin chain demonstration complete!\n");
    
    /* Cleanup */
    plugin_system_cleanup();
    
    return 0;
}
