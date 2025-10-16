/* Example Plugin: Exclamation Adder
 * Demonstrates a simple plugin for the hello.c plugin system
 * This plugin adds extra exclamation marks to the message
 */

#include "../plugin.h"
#include <string.h>
#include <stdio.h>

/* Transform function: adds exclamation marks */
static char* exclamation_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    
    /* Check if message already ends with '!' */
    if (len > 0 && input[len - 1] == '!') {
        /* Add two more exclamation marks */
        snprintf(output, output_size, "%s!!", input);
    } else {
        /* Add three exclamation marks */
        snprintf(output, output_size, "%s!!!", input);
    }
    
    return output;
}

/* Plugin initialization - automatically called when plugin is loaded */
void plugin_exclamation_init(void) {
    plugin_register("exclamation", "Adds extra exclamation marks", exclamation_transform);
}
