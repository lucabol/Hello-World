/**
 * Example postprocessor plugin that adds a timestamp
 * This demonstrates how to create a plugin that runs after the main output
 */
#include "plugin_api.h"
#include <stdio.h>
#include <time.h>

/* Plugin information */
static const plugin_info_t plugin_info = {
    .name = "timestamp-postprocessor",
    .version = "1.0.0", 
    .description = "Adds a timestamp after main output",
    .type = PLUGIN_TYPE_POST_PROCESSOR,
    .api_version = PLUGIN_API_VERSION
};

/* Plugin functions */
const plugin_info_t* plugin_get_info(void) {
    return &plugin_info;
}

int plugin_init(void) {
    printf("[timestamp] Plugin initialized\n");
    return 0;
}

void plugin_process(void) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char time_string[26];
    
    strftime(time_string, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("\n[Generated at: %s]", time_string);
}

void plugin_cleanup(void) {
    printf("\n[timestamp] Plugin cleaned up\n");
}