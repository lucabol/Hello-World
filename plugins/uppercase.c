/* Uppercase Plugin - Transforms greeting to uppercase
 * 
 * Example plugin that demonstrates the hello.c plugin API.
 * This plugin converts the greeting message to uppercase letters.
 * 
 * Build:
 *   gcc -shared -fPIC -Wall -Wextra -o plugins/uppercase.so plugins/uppercase.c
 * 
 * Usage:
 *   HELLO_PLUGIN=./plugins/uppercase.so ./hello
 * 
 * Expected output:
 *   HELLO WORLD!
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../plugin.h"

/* Static buffer for transformed message */
static char transformed[256];

/* Plugin metadata */
PluginInfo plugin_info(void) {
    PluginInfo info;
    info.name = "Uppercase Plugin";
    info.version = "1.0.0";
    info.description = "Converts greeting message to uppercase";
    info.api_version = PLUGIN_API_VERSION;
    return info;
}

/* Plugin initialization (optional) */
int plugin_init(void) {
    /* Could initialize resources here if needed */
    return 0;
}

/* Transform message to uppercase */
const char* plugin_transform(const char* message) {
    if (!message) {
        return NULL;
    }
    
    size_t len = strlen(message);
    if (len >= sizeof(transformed)) {
        fprintf(stderr, "Uppercase plugin: message too long\n");
        return message; /* Return original on error */
    }
    
    /* Convert to uppercase */
    for (size_t i = 0; i < len; i++) {
        transformed[i] = (char)toupper((unsigned char)message[i]);
    }
    transformed[len] = '\0';
    
    return transformed;
}

/* Plugin cleanup (optional) */
void plugin_cleanup(void) {
    /* Could cleanup resources here if needed */
}
