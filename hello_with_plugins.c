/* Hello with Plugins
 * SPDX-License-Identifier: MIT
 * 
 * This demonstrates integrating the plugin system with hello.c
 * External developers can register plugins to transform the greeting
 * without modifying hello.c source code.
 */

#include <stdio.h>
#include "plugin.h"

/* External plugin declarations - these would typically come from plugin modules */
extern plugin_t uppercase_plugin;
extern plugin_t exclaim_plugin;

int main(void) {
    const char* message = "Hello world!";
    char output[PLUGIN_MAX_MESSAGE_LEN];
    
    printf("=== Hello.c with Plugin System ===\n\n");
    
    /* Example 1: Original behavior (no plugins) */
    printf("1. Original (no plugins):\n   ");
    apply_plugins(message, output, sizeof(output));
    printf("%s\n\n", output);
    
    /* Example 2: With uppercase plugin */
    register_plugin(&uppercase_plugin);
    printf("2. With uppercase plugin:\n   ");
    apply_plugins(message, output, sizeof(output));
    printf("%s\n\n", output);
    
    /* Example 3: With uppercase + exclaim plugins (chained) */
    register_plugin(&exclaim_plugin);
    printf("3. With uppercase + exclaim plugins:\n   ");
    apply_plugins(message, output, sizeof(output));
    printf("%s\n\n", output);
    
    printf("===================================\n");
    printf("\nPlugin system allows extending hello.c functionality\n");
    printf("without modifying the core source code!\n");
    
    return 0;
}
