/* Uppercase Plugin
 * 
 * This plugin transforms the greeting message to uppercase.
 * Example plugin demonstrating the plugin system.
 */
#include "../plugin.h"
#include <ctype.h>
#include <string.h>

/* Plugin initialization (optional) */
static int uppercase_init(void) {
    /* No initialization needed for this simple plugin */
    return PLUGIN_SUCCESS;
}

/* Transform the greeting to uppercase */
static int uppercase_transform(const char* input, char* output, size_t output_size) {
    if (!input || !output || output_size == 0) {
        return PLUGIN_ERROR;
    }
    
    size_t len = strlen(input);
    if (len >= output_size) {
        return PLUGIN_ERROR;
    }
    
    /* Convert each character to uppercase */
    size_t i;
    for (i = 0; i < len && i < output_size - 1; i++) {
        output[i] = (char)toupper((unsigned char)input[i]);
    }
    output[i] = '\0';
    
    return PLUGIN_SUCCESS;
}

/* Plugin cleanup (optional) */
static int uppercase_cleanup(void) {
    /* No cleanup needed for this simple plugin */
    return PLUGIN_SUCCESS;
}

/* Plugin information - this must be exported */
plugin_info_t plugin_info = {
    .api_version = PLUGIN_API_VERSION,
    .name = "uppercase",
    .version = "1.0.0",
    .init = uppercase_init,
    .transform = uppercase_transform,
    .cleanup = uppercase_cleanup
};
