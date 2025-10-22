/* Plugin System Demo
 * Demonstrates how to use the plugin system with hello.c
 * 
 * This demo shows:
 * 1. How to initialize the plugin system
 * 2. How to register plugins
 * 3. How to apply plugins to transform output
 * 
 * Build:
 *   gcc -o plugin_demo plugin_demo.c plugin.c plugins/uppercase.c plugins/reverse.c plugins/prefix.c
 * 
 * Run:
 *   ./plugin_demo
 */

#include <stdio.h>
#include "plugin.h"

/* External plugin declarations */
extern const plugin_t uppercase_plugin;
extern const plugin_t reverse_plugin;
extern const plugin_t prefix_plugin;

int main(void) {
    const char* message = "Hello world!";
    const char* result;
    
    printf("=== Plugin System Demo ===\n\n");
    
    /* Original message */
    printf("Original message: %s\n\n", message);
    
    /* Demo 1: No plugins */
    printf("Demo 1: No plugins\n");
    plugin_init();
    result = plugin_apply_all(message);
    printf("Result: %s\n\n", result);
    
    /* Demo 2: Uppercase plugin */
    printf("Demo 2: Uppercase plugin\n");
    plugin_init();
    plugin_register(&uppercase_plugin);
    result = plugin_apply_all(message);
    printf("Result: %s\n\n", result);
    
    /* Demo 3: Reverse plugin */
    printf("Demo 3: Reverse plugin\n");
    plugin_init();
    plugin_register(&reverse_plugin);
    result = plugin_apply_all(message);
    printf("Result: %s\n\n", result);
    
    /* Demo 4: Prefix plugin */
    printf("Demo 4: Prefix plugin\n");
    plugin_init();
    plugin_register(&prefix_plugin);
    result = plugin_apply_all(message);
    printf("Result: %s\n\n", result);
    
    /* Demo 5: Multiple plugins (prefix + uppercase) */
    printf("Demo 5: Multiple plugins (prefix -> uppercase)\n");
    plugin_init();
    plugin_register(&prefix_plugin);
    plugin_register(&uppercase_plugin);
    result = plugin_apply_all(message);
    printf("Result: %s\n\n", result);
    
    /* Demo 6: All plugins chained */
    printf("Demo 6: All plugins chained (prefix -> uppercase -> reverse)\n");
    plugin_init();
    plugin_register(&prefix_plugin);
    plugin_register(&uppercase_plugin);
    plugin_register(&reverse_plugin);
    result = plugin_apply_all(message);
    printf("Result: %s\n\n", result);
    
    printf("=== Demo Complete ===\n");
    
    return 0;
}
