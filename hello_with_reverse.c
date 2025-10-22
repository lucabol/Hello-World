/* hello_with_reverse.c
 * Demonstrates using the reverse plugin to flip the greeting message
 */

#include <stdio.h>
#include "plugin.h"
#include "example_plugins.h"

static const char* greeting = "Hello world!";

/* Configure to use the reverse plugin */
static plugin_t plugins[] = {
    {"reverse", "Reverse the message", reverse_plugin}
};
static int plugin_count = sizeof(plugins) / sizeof(plugins[0]);

int main(){
    const char* final_message = apply_plugins(greeting, plugins, plugin_count);
    printf("%s\n", final_message);
    return 0;
}
