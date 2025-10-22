/* Prefix Plugin
 * Adds a prefix to the input message
 * 
 * Example:
 *   Input:  "Hello world!"
 *   Output: ">>> Hello world!"
 */

#include "../plugin.h"
#include <string.h>

static char prefix_buffer[PLUGIN_BUFFER_SIZE];

const char* prefix_transform(const char* input) {
    const char* prefix = ">>> ";
    size_t prefix_len;
    size_t input_len;
    size_t total_len;
    
    /* Validate input */
    if (input == NULL) {
        return NULL;
    }
    
    prefix_len = strlen(prefix);
    input_len = strlen(input);
    total_len = prefix_len + input_len;
    
    /* Ensure we don't overflow the buffer */
    if (total_len >= PLUGIN_BUFFER_SIZE) {
        total_len = PLUGIN_BUFFER_SIZE - 1;
        input_len = total_len - prefix_len;
    }
    
    /* Copy prefix and input */
    memcpy(prefix_buffer, prefix, prefix_len);
    memcpy(prefix_buffer + prefix_len, input, input_len);
    prefix_buffer[total_len] = '\0';
    
    return prefix_buffer;
}

/* Plugin descriptor */
const plugin_t prefix_plugin = {
    .name = "prefix",
    .transform = prefix_transform
};
