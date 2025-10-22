/* Example plugins for the hello.c plugin system
 * These demonstrate how to create plugins that transform the greeting message
 */

#include <string.h>
#include <ctype.h>

/* Buffer for plugin output - static to ensure lifetime beyond function scope */
static char plugin_output_buffer[256];

/* Uppercase Plugin
 * Transforms all characters to uppercase
 */
const char* uppercase_plugin(const char* input) {
    int i;
    int len = (int)strlen(input);
    
    /* Ensure we don't overflow the buffer */
    if (len >= 256) {
        len = 255;
    }
    
    for (i = 0; i < len; i++) {
        plugin_output_buffer[i] = (char)toupper((unsigned char)input[i]);
    }
    plugin_output_buffer[len] = '\0';
    
    return plugin_output_buffer;
}

/* Reverse Plugin
 * Reverses the message string
 */
const char* reverse_plugin(const char* input) {
    int len = (int)strlen(input);
    int i;
    
    /* Ensure we don't overflow the buffer */
    if (len >= 256) {
        len = 255;
    }
    
    for (i = 0; i < len; i++) {
        plugin_output_buffer[i] = input[len - 1 - i];
    }
    plugin_output_buffer[len] = '\0';
    
    return plugin_output_buffer;
}

/* Exclaim Plugin
 * Adds exclamation marks for emphasis
 */
const char* exclaim_plugin(const char* input) {
    int len = (int)strlen(input);
    
    /* Ensure we don't overflow the buffer */
    if (len >= 253) {
        len = 253;
    }
    
    /* Copy input and add exclamations */
    strncpy(plugin_output_buffer, input, (size_t)len);
    plugin_output_buffer[len] = '!';
    plugin_output_buffer[len + 1] = '!';
    plugin_output_buffer[len + 2] = '\0';
    
    return plugin_output_buffer;
}
