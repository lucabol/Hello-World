/* Plugin System Demo
 * Demonstrates the hello.c plugin system
 * 
 * This program shows:
 * 1. Using hello.c without plugins (default behavior)
 * 2. Using a single plugin
 * 3. Using multiple plugins in sequence
 */

#include <stdio.h>
#include "../hello.h"
#include "../plugin.h"

/* Include plugins - they register automatically */
#include "../plugins/exclamation_plugin.h"
#include "../plugins/uppercase_plugin.h"

int main() {
    printf("=== Hello World Plugin System Demo ===\n\n");
    
    /* Get the base greeting */
    const char* greeting = get_greeting();
    printf("1. Base greeting: %s\n", greeting);
    
    /* Apply plugins */
    const char* transformed = apply_plugins(greeting);
    printf("2. With plugins:  %s\n", transformed);
    
    printf("\nPlugins applied (in order):\n");
    printf("  - exclamation_plugin: adds extra '!!' \n");
    printf("  - uppercase_plugin: converts to uppercase\n");
    
    printf("\nResult: exclamation first, then uppercase\n");
    printf("  'Hello world!' -> 'Hello world!!!' -> 'HELLO WORLD!!!'\n");
    
    return 0;
}
