/* Hello World with Plugins Demo
 * Demonstrates using the plugin system to extend hello.c functionality
 */

#include <stdio.h>
#include "hello.h"
#include "plugin.h"

/* Include plugin headers to automatically register them */
#include "plugins/uppercase_plugin.h"

int main(){
    const char* greeting = get_greeting();
    const char* transformed = apply_plugins(greeting);
    printf("%s", transformed);
    return 0;
}
