/* Reverse Plugin - Reverses the greeting message
 * 
 * Example plugin that demonstrates the hello.c plugin API.
 * This plugin reverses the order of characters in the greeting.
 * 
 * Build:
 *   gcc -shared -fPIC -Wall -Wextra -o plugins/reverse.so plugins/reverse.c
 * 
 * Usage:
 *   HELLO_PLUGIN=./plugins/reverse.so ./hello
 * 
 * Expected output:
 *   !dlrow olleH
 */

#include <stdio.h>
#include <string.h>
#include "../plugin.h"

/* Static buffer for transformed message */
static char transformed[256];

/* Plugin metadata */
PluginInfo plugin_info(void) {
    PluginInfo info;
    info.name = "Reverse Plugin";
    info.version = "1.0.0";
    info.description = "Reverses the greeting message";
    info.api_version = PLUGIN_API_VERSION;
    return info;
}

/* Plugin initialization (optional) */
int plugin_init(void) {
    return 0;
}

/* Transform message by reversing it */
const char* plugin_transform(const char* message) {
    if (!message) {
        return NULL;
    }
    
    size_t len = strlen(message);
    if (len >= sizeof(transformed)) {
        fprintf(stderr, "Reverse plugin: message too long\n");
        return message; /* Return original on error */
    }
    
    /* Reverse the string */
    for (size_t i = 0; i < len; i++) {
        transformed[i] = message[len - 1 - i];
    }
    transformed[len] = '\0';
    
    return transformed;
}

/* Plugin cleanup (optional) */
void plugin_cleanup(void) {
    /* Cleanup if needed */
}
