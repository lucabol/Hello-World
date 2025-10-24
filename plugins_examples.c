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
 */

#include "plugin.h"
#include <string.h>
#include <ctype.h>

/* Uppercase Plugin - Converts message to uppercase */
static int uppercase_transform(const char* input, char* output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return -1;
    }
    
    size_t i;
    size_t len = strlen(input);
    
    if (len >= output_size) {
        return -1;
    }
    
    for (i = 0; i < len; i++) {
        output[i] = (char)toupper((unsigned char)input[i]);
    }
    output[len] = '\0';
    
    return 0;
}

plugin_t uppercase_plugin = {
    .name = "uppercase",
    .transform = uppercase_transform
};

/* Reverse Plugin - Reverses the message */
static int reverse_transform(const char* input, char* output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return -1;
    }
    
    size_t len = strlen(input);
    
    if (len >= output_size) {
        return -1;
    }
    
    size_t i;
    for (i = 0; i < len; i++) {
        output[i] = input[len - 1 - i];
    }
    output[len] = '\0';
    
    return 0;
}

plugin_t reverse_plugin = {
    .name = "reverse",
    .transform = reverse_transform
};

/* Exclaim Plugin - Adds extra exclamation marks */
static int exclaim_transform(const char* input, char* output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return -1;
    }
    
    size_t len = strlen(input);
    const char* suffix = "!!!";
    size_t suffix_len = strlen(suffix);
    
    if (len + suffix_len >= output_size) {
        return -1;
    }
    
    strcpy(output, input);
    strcat(output, suffix);
    
    return 0;
}

plugin_t exclaim_plugin = {
    .name = "exclaim",
    .transform = exclaim_transform
};

/* Prefix Plugin - Adds a prefix to the message */
static int prefix_transform(const char* input, char* output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return -1;
    }
    
    const char* prefix = "[Plugin] ";
    size_t prefix_len = strlen(prefix);
    size_t input_len = strlen(input);
    
    if (prefix_len + input_len >= output_size) {
        return -1;
    }
    
    strcpy(output, prefix);
    strcat(output, input);
    
    return 0;
}

plugin_t prefix_plugin = {
    .name = "prefix",
    .transform = prefix_transform
};
