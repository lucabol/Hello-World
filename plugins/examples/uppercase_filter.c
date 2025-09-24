/**
 * Example output filter plugin that converts text to uppercase
 * This demonstrates how to create a plugin that modifies the output
 */
#include "plugin_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Plugin information */
static const plugin_info_t plugin_info = {
    .name = "uppercase-filter",
    .version = "1.0.0",
    .description = "Converts output text to uppercase",
    .type = PLUGIN_TYPE_OUTPUT_FILTER,
    .api_version = PLUGIN_API_VERSION
};

/* Plugin functions */
const plugin_info_t* plugin_get_info(void) {
    return &plugin_info;
}

int plugin_init(void) {
    printf("[uppercase-filter] Plugin initialized\n");
    return 0;
}

char* plugin_filter(const char* input) {
    if (!input) {
        return NULL;
    }
    
    size_t len = strlen(input);
    char* result = malloc(len + 1);
    if (!result) {
        return NULL;
    }
    
    for (size_t i = 0; i < len; i++) {
        result[i] = (char)toupper((unsigned char)input[i]);
    }
    result[len] = '\0';
    
    printf("[uppercase-filter] Converted '%s' to '%s'\n", input, result);
    return result;
}

void plugin_cleanup(void) {
    printf("[uppercase-filter] Plugin cleaned up\n");
}