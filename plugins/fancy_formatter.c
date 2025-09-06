#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../plugin.h"

static plugin_info_t plugin_info = {
    .name = "fancy_formatter",
    .version = "1.0.0", 
    .description = "Formats output with decorative borders",
    .type = PLUGIN_TYPE_OUTPUT_FORMATTER,
    .api_version = PLUGIN_API_VERSION
};

static plugin_info_t* get_info(void) {
    return &plugin_info;
}

static int init(void) {
    return 0; /* Success */
}

static void format_output(const char* message) {
    if (!message) return;
    
    size_t len = strlen(message);
    
    /* Print top border */
    printf("*");
    for (size_t i = 0; i < len + 2; i++) {
        printf("*");
    }
    printf("*\n");
    
    /* Print message with side borders */
    printf("* %s *\n", message);
    
    /* Print bottom border */
    printf("*");
    for (size_t i = 0; i < len + 2; i++) {
        printf("*");
    }
    printf("*");
}

static void cleanup(void) {
    /* Nothing to cleanup */
}

static plugin_interface_t interface = {
    .get_info = get_info,
    .init = init,
    .transform_message = NULL,
    .format_output = format_output,
    .cleanup = cleanup
};

plugin_interface_t* get_plugin_interface(void) {
    return &interface;
}