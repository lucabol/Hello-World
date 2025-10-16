/* Uppercase Plugin
 * 
 * This plugin transforms the greeting message to uppercase.
 * Demonstrates a simple text transformation plugin.
 * 
 * Build:
 *   gcc -shared -fPIC -o uppercase_plugin.so uppercase_plugin.c
 * 
 * Use:
 *   HELLO_PLUGINS=./uppercase_plugin.so ./hello
 */

#include "../plugin_api.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Plugin initialization */
int plugin_init(void) {
    return 0;  /* Success */
}

/* Transform message to uppercase */
char* plugin_transform(const char* input) {
    if (input == NULL) {
        return NULL;
    }
    
    size_t len = strlen(input);
    char* output = (char*)malloc(len + 1);
    if (output == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < len; i++) {
        output[i] = (char)toupper((unsigned char)input[i]);
    }
    output[len] = '\0';
    
    return output;
}
