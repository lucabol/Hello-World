#include <stdio.h>
#include <string.h>
#include "../plugin.h"

/* Exclamation transformer - adds extra exclamation marks */
static int exclamation_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    
    /* Check if we have room for the extra exclamation marks */
    if (len + 2 >= output_size) {
        return -1;  /* Buffer too small */
    }
    
    /* Copy the input and add exclamation marks */
    strcpy(output, input);
    strcat(output, "!!");
    
    return 0;
}

/* Plugin initialization function */
plugin_info_t plugin_init(void) {
    plugin_info_t info = {
        .name = "exclamation",
        .version = PLUGIN_API_VERSION,
        .description = "Adds extra exclamation marks to the message"
    };
    
    /* Register our transformer */
    register_transformer("exclamation", exclamation_transform);
    
    return info;
}