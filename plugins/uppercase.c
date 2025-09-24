#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../plugin.h"

/* Uppercase transformer - converts message to uppercase */
static int uppercase_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    if (len >= output_size) {
        return -1;  /* Buffer too small */
    }
    
    for (size_t i = 0; i <= len; i++) {
        output[i] = (char)toupper((unsigned char)input[i]);
    }
    
    return 0;
}

/* Plugin initialization function */
plugin_info_t plugin_init(void) {
    plugin_info_t info = {
        .name = "uppercase",
        .version = PLUGIN_API_VERSION,
        .description = "Converts message to uppercase"
    };
    
    /* Register our transformer */
    register_transformer("uppercase", uppercase_transform);
    
    return info;
}