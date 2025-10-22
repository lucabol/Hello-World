/* Reverse Plugin
 * Reverses the input message
 * 
 * Example:
 *   Input:  "Hello world!"
 *   Output: "!dlrow olleH"
 */

#include "../plugin.h"
#include <string.h>

static char reverse_buffer[PLUGIN_BUFFER_SIZE];

const char* reverse_transform(const char* input) {
    size_t len;
    size_t i;
    
    /* Validate input */
    if (input == NULL) {
        return NULL;
    }
    
    len = strlen(input);
    
    /* Ensure we don't overflow the buffer */
    if (len >= PLUGIN_BUFFER_SIZE) {
        len = PLUGIN_BUFFER_SIZE - 1;
    }
    
    /* Reverse the string */
    for (i = 0; i < len; i++) {
        reverse_buffer[i] = input[len - 1 - i];
    }
    reverse_buffer[len] = '\0';
    
    return reverse_buffer;
}

/* Plugin descriptor */
const plugin_t reverse_plugin = {
    .name = "reverse",
    .transform = reverse_transform
};
