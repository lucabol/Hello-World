/* Example Plugin: Exclamation Adder
 * Demonstrates a simple plugin that adds extra exclamation marks
 * 
 * Usage: Include this header in your main file before the plugin system is used
 */

#ifndef EXCLAMATION_PLUGIN_H
#define EXCLAMATION_PLUGIN_H

#include "../plugin.h"

/* Static buffer for transformed message */
static char exclamation_buffer[256];

/* Plugin function: adds extra exclamation marks */
static const char* add_exclamation(const char* message) {
    int len = 0;
    while (message[len] != '\0' && len < 250) {
        exclamation_buffer[len] = message[len];
        len++;
    }
    /* Add extra exclamation marks */
    if (len < 253) {
        exclamation_buffer[len++] = '!';
        exclamation_buffer[len++] = '!';
        exclamation_buffer[len] = '\0';
    }
    return exclamation_buffer;
}

/* Register the plugin */
DEFINE_PLUGIN(add_exclamation)

#endif /* EXCLAMATION_PLUGIN_H */
