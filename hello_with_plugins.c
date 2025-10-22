/* hello_with_plugins.c
 * Demonstration of the plugin system for hello.c
 * This file shows how to use plugins to transform the greeting message
 * 
 * To use plugins:
 * 1. Include the plugin header files
 * 2. Add plugin functions to the plugins array
 * 3. Set plugin_count to the number of active plugins
 * 4. Compile with plugin source files
 * 
 * Example: gcc -o hello-with-uppercase hello_with_plugins.c example_plugins.c
 */

#include <stdio.h>
#include "plugin.h"
#include "example_plugins.h"

/* Default greeting message */
static const char* greeting = "Hello world!";

/* Plugin array - demonstrates using the uppercase plugin */
static plugin_t plugins[] = {
    {"uppercase", "Convert message to uppercase", uppercase_plugin}
};
static int plugin_count = sizeof(plugins) / sizeof(plugins[0]);

int main(){
    /* Apply plugins to greeting */
    const char* final_message = apply_plugins(greeting, plugins, plugin_count);
    
    printf("%s\n", final_message);
    return 0;
}
