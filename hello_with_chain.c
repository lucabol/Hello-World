/* hello_with_chain.c
 * Demonstrates chaining multiple plugins together
 * The output of one plugin becomes the input to the next
 */

#include <stdio.h>
#include "plugin.h"
#include "example_plugins.h"

static const char* greeting = "Hello world!";

/* Chain multiple plugins: reverse, then uppercase */
static plugin_t plugins[] = {
    {"reverse", "Reverse the message", reverse_plugin},
    {"uppercase", "Convert to uppercase", uppercase_plugin}
};
static int plugin_count = sizeof(plugins) / sizeof(plugins[0]);

int main(){
    const char* final_message = apply_plugins(greeting, plugins, plugin_count);
    
    /* This will output: !DLROW OLLEH (reversed, then uppercased) */
    printf("%s\n", final_message);
    return 0;
}
