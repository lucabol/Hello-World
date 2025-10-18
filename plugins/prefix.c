/* Prefix Plugin
 * 
 * This plugin adds a prefix to the greeting message.
 * Demonstrates a practical plugin use case.
 */
#include "../plugin.h"
#include <string.h>
#include <stdio.h>

/* The prefix to add */
#define PREFIX ">> "

/* Transform the greeting by adding a prefix */
static int prefix_transform(const char* input, char* output, size_t output_size) {
    if (!input || !output || output_size == 0) {
        return PLUGIN_ERROR;
    }
    
    size_t prefix_len = strlen(PREFIX);
    size_t input_len = strlen(input);
    
    /* Check if there's enough space for prefix + input + null terminator */
    if (prefix_len + input_len + 1 > output_size) {
        return PLUGIN_ERROR;
    }
    
    /* Copy prefix and then input */
    strncpy(output, PREFIX, output_size - 1);
    strncat(output, input, output_size - prefix_len - 1);
    output[output_size - 1] = '\0';
    
    return PLUGIN_SUCCESS;
}

/* Plugin information - this must be exported */
plugin_info_t plugin_info = {
    .api_version = PLUGIN_API_VERSION,
    .name = "prefix",
    .version = "1.0.0",
    .init = NULL,  /* No initialization needed */
    .transform = prefix_transform,
    .cleanup = NULL  /* No cleanup needed */
};
