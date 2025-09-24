/**
 * Example preprocessor plugin that prints a banner
 * This demonstrates how to create a plugin that runs before the main output
 */
#include "plugin_api.h"
#include <stdio.h>

/* Plugin information */
static const plugin_info_t plugin_info = {
    .name = "banner-preprocessor",
    .version = "1.0.0",
    .description = "Prints a banner before main output",
    .type = PLUGIN_TYPE_PRE_PROCESSOR,
    .api_version = PLUGIN_API_VERSION
};

/* Plugin functions */
const plugin_info_t* plugin_get_info(void) {
    return &plugin_info;
}

int plugin_init(void) {
    printf("[banner] Plugin initialized\n");
    return 0;
}

void plugin_process(void) {
    printf("*** Welcome to Hello World with Plugins! ***\n");
}

void plugin_cleanup(void) {
    printf("[banner] Plugin cleaned up\n");
}