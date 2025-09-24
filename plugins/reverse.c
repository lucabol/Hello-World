#include <stdio.h>
#include <string.h>
#include "../plugin.h"

/* Reverse transformer - reverses the message */
static int reverse_transform(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    if (len >= output_size) {
        return -1;  /* Buffer too small */
    }
    
    /* Copy characters in reverse order */
    for (size_t i = 0; i < len; i++) {
        output[i] = input[len - 1 - i];
    }
    output[len] = '\0';
    
    return 0;
}

/* Plugin initialization function */
plugin_info_t plugin_init(void) {
    plugin_info_t info = {
        .name = "reverse",
        .version = PLUGIN_API_VERSION,
        .description = "Reverses the message character order"
    };
    
    /* Register our transformer */
    register_transformer("reverse", reverse_transform);
    
    return info;
}