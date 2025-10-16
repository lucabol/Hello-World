/* Hello World with Plugin Support
 * This version enables the plugin system
 * 
 * Compile without plugins:
 *   gcc -o hello hello_plugin_enabled.c plugin.c
 * 
 * Compile with uppercase plugin:
 *   gcc -o hello hello_plugin_enabled.c plugin.c plugins/example_uppercase.c -DLOAD_UPPERCASE
 * 
 * Compile with reverse plugin:
 *   gcc -o hello hello_plugin_enabled.c plugin.c plugins/example_reverse.c -DLOAD_REVERSE
 * 
 * Compile with both plugins:
 *   gcc -o hello hello_plugin_enabled.c plugin.c plugins/example_uppercase.c plugins/example_reverse.c -DLOAD_UPPERCASE -DLOAD_REVERSE
 */

#include <stdio.h>
#include "plugin.h"

/* Plugin initialization functions (defined in plugin files) */
#ifdef LOAD_UPPERCASE
void plugin_uppercase_init(void);
#endif

#ifdef LOAD_REVERSE
void plugin_reverse_init(void);
#endif

int main(void) {
    const char* message = "Hello world!";
    char output[256];
    
    /* Initialize plugins */
#ifdef LOAD_UPPERCASE
    plugin_uppercase_init();
#endif

#ifdef LOAD_REVERSE
    plugin_reverse_init();
#endif
    
    /* Apply plugins and print result */
    plugin_apply_all(message, output, sizeof(output));
    printf("%s", output);
    
    return 0;
}
