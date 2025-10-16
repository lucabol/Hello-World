/* Reverse Plugin
 * 
 * This plugin reverses the greeting message.
 * Demonstrates a more complex transformation plugin.
 * 
 * Build:
 *   gcc -shared -fPIC -o reverse_plugin.so reverse_plugin.c
 * 
 * Use:
 *   HELLO_PLUGINS=./reverse_plugin.so ./hello
 */

#include "../plugin_api.h"
#include <stdlib.h>
#include <string.h>

/* Plugin initialization */
int plugin_init(void) {
    return 0;  /* Success */
}

/* Transform message by reversing it */
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
        output[i] = input[len - 1 - i];
    }
    output[len] = '\0';
    
    return output;
}
