/* Plugin System Demo
 * 
 * This program demonstrates how the plugin system extends hello.c functionality
 * without modifying the core file.
 * 
 * It shows:
 * - How to register plugins
 * - How plugins transform messages
 * - How multiple plugins can be chained
 */

#include <stdio.h>
#include "plugin.h"

/* External plugin declarations from plugins_examples.c */
extern plugin_t uppercase_plugin;
extern plugin_t reverse_plugin;
extern plugin_t exclaim_plugin;
extern plugin_t prefix_plugin;

int main(void) {
    const char* original_message = "Hello world!";
    char output[PLUGIN_MAX_MESSAGE_LEN];
    int result;
    
    printf("Plugin System Demonstration\n");
    printf("============================\n\n");
    
    /* Test 1: No plugins (should output original message) */
    printf("Test 1: No plugins\n");
    printf("  Input:  '%s'\n", original_message);
    result = apply_plugins(original_message, output, sizeof(output));
    printf("  Output: '%s'\n", output);
    printf("  Plugins applied: %d\n\n", result);
    
    /* Test 2: Single plugin - Uppercase */
    clear_plugins();
    register_plugin(&uppercase_plugin);
    printf("Test 2: Uppercase plugin\n");
    printf("  Input:  '%s'\n", original_message);
    result = apply_plugins(original_message, output, sizeof(output));
    printf("  Output: '%s'\n", output);
    printf("  Plugins applied: %d\n\n", result);
    
    /* Test 3: Single plugin - Reverse */
    clear_plugins();
    register_plugin(&reverse_plugin);
    printf("Test 3: Reverse plugin\n");
    printf("  Input:  '%s'\n", original_message);
    result = apply_plugins(original_message, output, sizeof(output));
    printf("  Output: '%s'\n", output);
    printf("  Plugins applied: %d\n\n", result);
    
    /* Test 4: Single plugin - Exclaim */
    clear_plugins();
    register_plugin(&exclaim_plugin);
    printf("Test 4: Exclaim plugin\n");
    printf("  Input:  '%s'\n", original_message);
    result = apply_plugins(original_message, output, sizeof(output));
    printf("  Output: '%s'\n", output);
    printf("  Plugins applied: %d\n\n", result);
    
    /* Test 5: Chained plugins - Uppercase + Exclaim */
    clear_plugins();
    register_plugin(&uppercase_plugin);
    register_plugin(&exclaim_plugin);
    printf("Test 5: Uppercase + Exclaim (chained)\n");
    printf("  Input:  '%s'\n", original_message);
    result = apply_plugins(original_message, output, sizeof(output));
    printf("  Output: '%s'\n", output);
    printf("  Plugins applied: %d\n\n", result);
    
    /* Test 6: Complex chain - Prefix + Uppercase + Exclaim */
    clear_plugins();
    register_plugin(&prefix_plugin);
    register_plugin(&uppercase_plugin);
    register_plugin(&exclaim_plugin);
    printf("Test 6: Prefix + Uppercase + Exclaim (complex chain)\n");
    printf("  Input:  '%s'\n", original_message);
    result = apply_plugins(original_message, output, sizeof(output));
    printf("  Output: '%s'\n", output);
    printf("  Plugins applied: %d\n\n", result);
    
    /* Test 7: Reverse + Uppercase (order matters) */
    clear_plugins();
    register_plugin(&reverse_plugin);
    register_plugin(&uppercase_plugin);
    printf("Test 7: Reverse + Uppercase (order matters)\n");
    printf("  Input:  '%s'\n", original_message);
    result = apply_plugins(original_message, output, sizeof(output));
    printf("  Output: '%s'\n", output);
    printf("  Plugins applied: %d\n\n", result);
    
    printf("============================\n");
    printf("All tests completed!\n");
    
    return 0;
}
