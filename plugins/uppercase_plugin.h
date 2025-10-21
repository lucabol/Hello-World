/* Example Plugin: Uppercase Transformer
 * Demonstrates how to create a plugin for hello.c
 * This plugin transforms the greeting to uppercase
 * 
 * Usage: Include this header in your main file before the plugin system is used
 */

#ifndef UPPERCASE_PLUGIN_H
#define UPPERCASE_PLUGIN_H

#include <ctype.h>
#include "plugin.h"

/* Static buffer for transformed message - use unique name to avoid collisions */
static char PLUGIN_BUFFER(uppercase)[256];

/* Plugin function: transforms message to uppercase */
static const char* uppercase_transform(const char* message) {
    int i = 0;
    while (message[i] != '\0' && i < 255) {
        PLUGIN_BUFFER(uppercase)[i] = (char)toupper((unsigned char)message[i]);
        i++;
    }
    PLUGIN_BUFFER(uppercase)[i] = '\0';
    return PLUGIN_BUFFER(uppercase);
}

/* Register the plugin using the DEFINE_PLUGIN macro */
DEFINE_PLUGIN(uppercase_transform)

#endif /* UPPERCASE_PLUGIN_H */
