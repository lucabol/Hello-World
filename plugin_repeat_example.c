/* Example: Creating Your Own Custom Plugin
 * 
 * This file demonstrates how to create a custom plugin from scratch.
 * You can use this as a template for your own plugins.
 * 
 * This example creates a "Repeat" plugin that prints the message multiple times.
 */

#include "plugin.h"
#include <stdio.h>
#include <string.h>

/* Configuration: Number of times to repeat (could be from environment variable) */
#define REPEAT_COUNT 3

/* Transform function - repeats the message */
static int repeat_transform(const char* input, char* output, size_t output_size) {
    int i;
    size_t offset = 0;
    size_t input_len = strlen(input);
    int result;
    
    /* Clear output buffer */
    output[0] = '\0';
    
    /* Repeat the message */
    for (i = 0; i < REPEAT_COUNT; i++) {
        if (i > 0) {
            /* Add separator between repetitions */
            result = snprintf(output + offset, output_size - offset, " | ");
            if (result < 0 || offset + (size_t)result >= output_size) {
                return -1; /* Error: buffer too small */
            }
            offset += (size_t)result;
        }
        
        result = snprintf(output + offset, output_size - offset, "%s", input);
        if (result < 0 || offset + (size_t)result >= output_size) {
            return -1; /* Error: buffer too small */
        }
        offset += (size_t)result;
    }
    
    return 0; /* Success */
}

/* Before hook - announces the repetition count */
static void repeat_before(void) {
    printf("[Repeat Plugin: Will repeat message %d times]\n", REPEAT_COUNT);
}

/* After hook - adds a completion message */
static void repeat_after(void) {
    printf("\n[Repeat Plugin: Done]\n");
}

/* Register the plugin */
PLUGIN_REGISTER(RepeatPlugin, repeat_transform, repeat_before, repeat_after);

/* 
 * To use this plugin:
 * 
 * 1. Save this file as plugin_repeat.c
 * 
 * 2. Compile with hello.c:
 *    gcc -DUSE_PLUGINS -o hello_repeat hello.c plugin.c plugin_repeat.c
 * 
 * 3. Run:
 *    ./hello_repeat
 * 
 * Expected output:
 *    [Repeat Plugin: Will repeat message 3 times]
 *    Hello world! | Hello world! | Hello world!
 *    [Repeat Plugin: Done]
 * 
 * Customization ideas:
 * - Read REPEAT_COUNT from an environment variable
 * - Add different separators between repetitions
 * - Apply different formatting to each repetition
 * - Count and display statistics about the message
 */
