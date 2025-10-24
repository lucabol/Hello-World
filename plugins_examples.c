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

/* Safe string copy helper - always NUL-terminates */
static int safe_copy(char* dest, const char* src, size_t dest_size) {
    size_t src_len;
    
    if (dest == NULL || src == NULL || dest_size == 0) {
        return -1;
    }
    
    src_len = strlen(src);
    
    if (src_len >= dest_size) {
        if (dest_size > 0) {
            memcpy(dest, src, dest_size - 1);
            dest[dest_size - 1] = '\0';
        }
        return -1;
    }
    
    memcpy(dest, src, src_len + 1);
    return 0;
}

/* Safe string concatenation helper - always NUL-terminates */
static int safe_concat(char* dest, const char* src, size_t dest_size) {
    size_t dest_len = strlen(dest);
    size_t src_len = strlen(src);
    size_t available;
    
    if (dest == NULL || src == NULL || dest_size == 0 || dest_len >= dest_size) {
        return -1;
    }
    
    available = dest_size - dest_len - 1;
    
    if (src_len > available) {
        memcpy(dest + dest_len, src, available);
        dest[dest_size - 1] = '\0';
        return -1;
    }
    
    memcpy(dest + dest_len, src, src_len + 1);
    return 0;
}

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
    const char* suffix = "!!!";
    
    /* Check buffer size */
    if (strlen(input) + strlen(suffix) >= output_size) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    /* Use safe string operations */
    if (safe_copy(output, input, output_size) != 0) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    /* Append suffix */
    if (safe_concat(output, suffix, output_size) != 0) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
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
    
    /* Check buffer size */
    if (strlen(prefix) + strlen(input) >= output_size) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    /* Use safe string operations */
    if (safe_copy(output, prefix, output_size) != 0) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    /* Append input */
    if (safe_concat(output, input, output_size) != 0) {
        return PLUGIN_ERR_BUFFER_TOO_SMALL;
    }
    
    return PLUGIN_SUCCESS;
}

plugin_t prefix_plugin = {
    .name = "prefix",
    .transform = prefix_transform
};
