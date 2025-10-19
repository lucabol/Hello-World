/* Example Plugin: Uppercase Transformer
 * Demonstrates how to create a plugin for hello.c
 * This plugin transforms the greeting to uppercase
 * 
 * Usage: Include this header in your main file before the plugin system is used
 */

#ifndef UPPERCASE_PLUGIN_H
#define UPPERCASE_PLUGIN_H

#include <ctype.h>
#include "../plugin.h"

/* Static buffer for transformed message */
static char uppercase_buffer[256];

/* Plugin function: transforms message to uppercase */
static const char* uppercase_transform(const char* message) {
    int i = 0;
    while (message[i] != '\0' && i < 255) {
        uppercase_buffer[i] = (char)toupper((unsigned char)message[i]);
        i++;
    }
    uppercase_buffer[i] = '\0';
    return uppercase_buffer;
}

/* Register the plugin using the DEFINE_PLUGIN macro */
DEFINE_PLUGIN(uppercase_transform)

#endif /* UPPERCASE_PLUGIN_H */
