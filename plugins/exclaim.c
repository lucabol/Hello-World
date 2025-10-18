/* Exclaim Plugin
 * 
 * This plugin adds extra exclamation marks to the greeting message.
 * Another example demonstrating the plugin system.
 */
#include "../plugin.h"
#include <string.h>
#include <stdio.h>

/* Transform the greeting by adding exclamation marks */
static int exclaim_transform(const char* input, char* output, size_t output_size) {
    if (!input || !output || output_size == 0) {
        return PLUGIN_ERROR;
    }
    
    size_t len = strlen(input);
    
    /* Check if there's enough space for input + "!!" + null terminator */
    if (len + 3 >= output_size) {
        return PLUGIN_ERROR;
    }
    
    /* Copy input and add two exclamation marks */
    strncpy(output, input, output_size - 3);
    output[len] = '!';
    output[len + 1] = '!';
    output[len + 2] = '\0';
    
    return PLUGIN_SUCCESS;
}

/* Plugin information - this must be exported */
plugin_info_t plugin_info = {
    .api_version = PLUGIN_API_VERSION,
    .name = "exclaim",
    .version = "1.0.0",
    .init = NULL,  /* No initialization needed */
    .transform = exclaim_transform,
    .cleanup = NULL  /* No cleanup needed */
};
