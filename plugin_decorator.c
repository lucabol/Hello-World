/* Example Plugin: Decorator
 * Demonstrates plugin hooks and message transformation
 * This plugin adds decorative borders around the message
 */

#include "plugin.h"
#include <stdio.h>
#include <string.h>

/* Transform function - adds asterisks around the message */
static int decorator_transform(const char* input, char* output, size_t output_size) {
    int result = snprintf(output, output_size, "*** %s ***", input);
    
    /* Check for errors or truncation */
    if (result < 0 || (size_t)result >= output_size) {
        return -1; /* Error: buffer too small */
    }
    
    return 0; /* Success */
}

/* Before hook - prints a header */
static void decorator_before(void) {
    printf("=== Plugin Output Start ===\n");
}

/* After hook - prints a footer */
static void decorator_after(void) {
    printf("\n=== Plugin Output End ===\n");
}

/* Register the plugin using the convenience macro */
PLUGIN_REGISTER(DecoratorPlugin, decorator_transform, decorator_before, decorator_after);
