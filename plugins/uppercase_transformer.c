#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../plugin.h"

static plugin_info_t plugin_info = {
    .name = "uppercase_transformer",
    .version = "1.0.0",
    .description = "Transforms message to uppercase",
    .type = PLUGIN_TYPE_MESSAGE_TRANSFORMER,
    .api_version = PLUGIN_API_VERSION
};

static plugin_info_t* get_info(void) {
    return &plugin_info;
}

static int init(void) {
    return 0; /* Success */
}

static char* transform_message(const char* input) {
    if (!input) return NULL;
    
    size_t len = strlen(input);
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = toupper(input[i]);
    }
    result[len] = '\0';
    
    return result;
}

static void cleanup(void) {
    /* Nothing to cleanup */
}

static plugin_interface_t interface = {
    .get_info = get_info,
    .init = init,
    .transform_message = transform_message,
    .format_output = NULL,
    .cleanup = cleanup
};

plugin_interface_t* get_plugin_interface(void) {
    return &interface;
}