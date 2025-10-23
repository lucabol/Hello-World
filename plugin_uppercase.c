/* Example Plugin: Uppercase Transformer
 * Demonstrates how to create a plugin for hello.c
 * This plugin transforms the message to uppercase
 */

#include "plugin.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Static buffer for transformed message */
static char uppercase_buffer[256];

/* Transform function - converts message to uppercase */
static const char* uppercase_transform(const char* input) {
    size_t i;
    size_t len = strlen(input);
    
    if (len >= sizeof(uppercase_buffer)) {
        len = sizeof(uppercase_buffer) - 1;
    }
    
    for (i = 0; i < len; i++) {
        uppercase_buffer[i] = (char)toupper((unsigned char)input[i]);
    }
    uppercase_buffer[len] = '\0';
    
    return uppercase_buffer;
}

/* Before hook - executes before the main message */
static void uppercase_before(void) {
    /* This hook is intentionally empty for this plugin */
    (void)0;
}

/* After hook - executes after the main message */
static void uppercase_after(void) {
    /* This hook is intentionally empty for this plugin */
    (void)0;
}

/* Register the plugin using the convenience macro */
PLUGIN_REGISTER(UppercasePlugin, uppercase_transform, uppercase_before, uppercase_after);
