/* Example Plugin: Uppercase Transformer
 * Demonstrates how to create a plugin for the hello.c plugin system
 * This plugin converts the message to uppercase
 */

#include "../plugin.h"
#include <ctype.h>
#include <string.h>

/* Transform function: converts message to uppercase */
static char* uppercase_transform(const char* input, char* output, size_t output_size) {
    size_t i;
    size_t len = strlen(input);
    
    if (len >= output_size) {
        len = output_size - 1;
    }
    
    for (i = 0; i < len; i++) {
        output[i] = (char)toupper((unsigned char)input[i]);
    }
    output[len] = '\0';
    
    return output;
}

/* Plugin initialization - automatically called when plugin is loaded */
void plugin_uppercase_init(void) {
    plugin_register("uppercase", "Converts message to uppercase", uppercase_transform);
}
