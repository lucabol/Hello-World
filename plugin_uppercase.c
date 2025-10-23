/* Example Plugin: Uppercase Transformer
 * Demonstrates how to create a plugin for hello.c
 * This plugin transforms the message to uppercase
 */

#include "plugin.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Transform function - converts message to uppercase */
static int uppercase_transform(const char* input, char* output, size_t output_size) {
    size_t i;
    size_t len = strlen(input);
    
    /* Check if output buffer is large enough */
    if (len >= output_size) {
        return -1; /* Error: buffer too small */
    }
    
    /* Transform to uppercase */
    for (i = 0; i < len; i++) {
        output[i] = (char)toupper((unsigned char)input[i]);
    }
    output[len] = '\0';
    
    return 0; /* Success */
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
