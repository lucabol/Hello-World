/* Example: hello.c with plugins enabled
 * Demonstrates how to use the plugin system to extend functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include "plugin.h"

/* Declare plugin functions */
extern char* uppercase_plugin(const char* input);
extern char* reverse_plugin(const char* input);

int main(){
    const char* message = "Hello world!";
    
    /* Register plugins - they will be applied in order */
    register_plugin(uppercase_plugin);
    
    /* Apply plugins and print result */
    char* transformed = apply_plugins(message);
    printf("%s", transformed);
    
    /* Free transformed message if it was allocated by plugins */
    if (transformed != message) {
        free(transformed);
    }
    
    return 0;
}
