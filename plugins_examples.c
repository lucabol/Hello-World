/* Example Plugins for hello.c
 * 
 * This file demonstrates how to create plugins that extend hello.c functionality
 * without modifying the core file.
 * 
 * Example plugins:
 * - uppercase_plugin: Converts message to uppercase
 * - reverse_plugin: Reverses the message
 * - exclaim_plugin: Adds extra exclamation marks
 * - prefix_plugin: Adds a prefix to the message
 * 
 * All examples use safe string operations and proper error handling.
 */

#include "plugin.h"
#include <string.h>
#include <ctype.h>

/* Uppercase Plugin - Converts message to uppercase */
static int uppercase_transform(const char* input, char* output, size_t output_size) {
    size_t i;
    size_t len = strlen(input);
    
    /* Check buffer size */
    if (len >= output_size) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    /* Transform each character */
    for (i = 0; i < len; i++) {
        output[i] = (char)toupper((unsigned char)input[i]);
    }
    output[len] = '\0';
    
    return PLUGIN_SUCCESS;
}

plugin_t uppercase_plugin = {
    .name = "uppercase",
    .transform = uppercase_transform
};

/* Reverse Plugin - Reverses the message */
static int reverse_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    size_t i;
    
    /* Check buffer size */
    if (len >= output_size) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    /* Reverse the string */
    for (i = 0; i < len; i++) {
        output[i] = input[len - 1 - i];
    }
    output[len] = '\0';
    
    return PLUGIN_SUCCESS;
}

plugin_t reverse_plugin = {
    .name = "reverse",
    .transform = reverse_transform
};

/* Exclaim Plugin - Adds extra exclamation marks */
static int exclaim_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    const char* suffix = "!!!";
    size_t suffix_len = strlen(suffix);
    
    /* Check buffer size */
    if (len + suffix_len >= output_size) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    /* Use safe string operations */
    strncpy(output, input, output_size - 1);
    output[output_size - 1] = '\0';
    
    /* Append suffix if there's room */
    if (len + suffix_len < output_size) {
        strncat(output, suffix, output_size - len - 1);
    }
    
    return PLUGIN_SUCCESS;
}

plugin_t exclaim_plugin = {
    .name = "exclaim",
    .transform = exclaim_transform
};

/* Prefix Plugin - Adds a prefix to the message */
static int prefix_transform(const char* input, char* output, size_t output_size) {
    const char* prefix = "[Plugin] ";
    size_t prefix_len = strlen(prefix);
    size_t input_len = strlen(input);
    
    /* Check buffer size */
    if (prefix_len + input_len >= output_size) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    /* Use safe string operations */
    strncpy(output, prefix, output_size - 1);
    output[output_size - 1] = '\0';
    
    /* Append input if there's room */
    if (prefix_len < output_size) {
        strncat(output, input, output_size - prefix_len - 1);
    }
    
    return PLUGIN_SUCCESS;
}

plugin_t prefix_plugin = {
    .name = "prefix",
    .transform = prefix_transform
};
