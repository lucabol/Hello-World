/* Bracket Plugin
 * 
 * This plugin wraps the greeting message in square brackets.
 * Demonstrates a simple formatting plugin.
 * 
 * Build:
 *   gcc -shared -fPIC -o bracket_plugin.so bracket_plugin.c
 * 
 * Use:
 *   HELLO_PLUGINS=./bracket_plugin.so ./hello
 */

#include "../plugin_api.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Plugin initialization */
int plugin_init(void) {
    return 0;  /* Success */
}

/* Transform message by adding brackets */
char* plugin_transform(const char* input) {
    if (input == NULL) {
        return NULL;
    }
    
    size_t len = strlen(input);
    /* Allocate space for brackets and null terminator: "[" + input + "]" + '\0' */
    char* output = (char*)malloc(len + 3);
    if (output == NULL) {
        return NULL;
    }
    
    sprintf(output, "[%s]", input);
    
    return output;
}
