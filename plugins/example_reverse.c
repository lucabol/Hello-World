/* Example Plugin: Reverse Transformer
 * Demonstrates how to create a plugin for the hello.c plugin system
 * This plugin reverses the message
 */

#include "../plugin.h"
#include <string.h>

/* Transform function: reverses the message */
static char* reverse_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    size_t i;
    
    if (len >= output_size) {
        len = output_size - 1;
    }
    
    for (i = 0; i < len; i++) {
        output[i] = input[len - 1 - i];
    }
    output[len] = '\0';
    
    return output;
}

/* Plugin initialization - automatically called when plugin is loaded */
void plugin_reverse_init(void) {
    plugin_register("reverse", "Reverses the message", reverse_transform);
}
