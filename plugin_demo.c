/* Plugin Demo - Shows how to use the plugin system
 * Compile: gcc -DENABLE_PLUGINS -o hello_with_plugins hello.c plugin.c plugins/example_uppercase.c
 * 
 * This demonstrates:
 * 1. Loading and using plugins
 * 2. Plugin initialization
 * 3. Message transformation
 */

#include <stdio.h>
#include "plugin.h"

/* External plugin initialization functions */
void plugin_uppercase_init(void);
void plugin_reverse_init(void);

int main(int argc, char* argv[]) {
    const char* message = "Hello world!";
    char output[256];
    
    printf("=== Plugin System Demo ===\n\n");
    
    /* Show original message */
    printf("Original message: %s\n\n", message);
    
    /* Demo 1: No plugins */
    printf("1. No plugins loaded:\n");
    plugin_apply_all(message, output, sizeof(output));
    printf("   Output: %s\n\n", output);
    
    /* Demo 2: Uppercase plugin */
    printf("2. With uppercase plugin:\n");
    plugin_uppercase_init();
    plugin_apply_all(message, output, sizeof(output));
    printf("   Output: %s\n\n", output);
    
    /* Demo 3: Adding reverse plugin (chaining) */
    printf("3. With uppercase + reverse plugins (chained):\n");
    plugin_reverse_init();
    plugin_apply_all(message, output, sizeof(output));
    printf("   Output: %s\n\n", output);
    
    printf("Total plugins loaded: %d\n", plugin_count());
    
    (void)argc;
    (void)argv;
    
    return 0;
}
