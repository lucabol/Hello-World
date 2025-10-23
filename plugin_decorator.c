/* Example Plugin: Decorator
 * Demonstrates plugin hooks and message transformation
 * This plugin adds decorative borders around the message
 */

#include "plugin.h"
#include <stdio.h>
#include <string.h>

/* Static buffer for decorated message */
static char decorated_buffer[256];

/* Transform function - adds asterisks around the message */
static const char* decorator_transform(const char* input) {
    snprintf(decorated_buffer, sizeof(decorated_buffer), "*** %s ***", input);
    return decorated_buffer;
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
