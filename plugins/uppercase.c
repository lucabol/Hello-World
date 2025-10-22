/* Uppercase Plugin
 * Transforms the input message to uppercase
 * 
 * Example:
 *   Input:  "Hello world!"
 *   Output: "HELLO WORLD!"
 */

#include "../plugin.h"
#include <ctype.h>
#include <string.h>

static char uppercase_buffer[PLUGIN_BUFFER_SIZE];

const char* uppercase_transform(const char* input) {
    size_t i;
    size_t len = strlen(input);
    
    /* Ensure we don't overflow the buffer */
    if (len >= PLUGIN_BUFFER_SIZE) {
        len = PLUGIN_BUFFER_SIZE - 1;
    }
    
    /* Convert each character to uppercase */
    for (i = 0; i < len; i++) {
        uppercase_buffer[i] = (char)toupper((unsigned char)input[i]);
    }
    uppercase_buffer[len] = '\0';
    
    return uppercase_buffer;
}

/* Plugin descriptor */
const plugin_t uppercase_plugin = {
    .name = "uppercase",
    .transform = uppercase_transform
};
