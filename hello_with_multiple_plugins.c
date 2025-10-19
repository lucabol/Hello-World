/* Hello World with Multiple Plugins Demo */

#include <stdio.h>
#include "hello.h"
#include "plugin.h"

/* Include plugin headers - they will be applied in order */
#include "plugins/exclamation_plugin.h"
#include "plugins/uppercase_plugin.h"

int main(){
    const char* greeting = get_greeting();
    const char* transformed = apply_plugins(greeting);
    printf("%s", transformed);
    return 0;
}
