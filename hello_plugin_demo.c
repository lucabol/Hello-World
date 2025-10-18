/* Hello World with Plugin System Demo
 * 
 * Demonstrates the plugin system by loading and applying plugins
 * to transform the greeting message.
 */
#include <stdio.h>
#include "plugin.h"

/* Original greeting message */
static const char* get_greeting(void) {
    return "Hello world!";
}

int main(void) {
    char output[256];
    
    /* Initialize the plugin system */
    plugin_system_init();
    
    /* Example 1: No plugins - original behavior */
    printf("Example 1: No plugins\n");
    plugin_apply_all(get_greeting(), output, sizeof(output));
    printf("%s\n\n", output);
    
    /* Example 2: With uppercase plugin */
    printf("Example 2: With uppercase plugin\n");
    extern plugin_info_t plugin_info;  /* From uppercase.c */
    plugin_register(&plugin_info);
    plugin_apply_all(get_greeting(), output, sizeof(output));
    printf("%s\n\n", output);
    
    /* Cleanup */
    plugin_system_cleanup();
    
    return 0;
}
