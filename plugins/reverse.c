/* Reverse Plugin - Reverses the message */

#include <stdlib.h>
#include <string.h>
#include "../plugin.h"

char* reverse_plugin(const char* input) {
    if (input == NULL) {
        return NULL;
    }
    
    size_t len = strlen(input);
    char* result = (char*)malloc(len + 1);
    
    if (result == NULL) {
        return (char*)input; /* Return original on allocation failure */
    }
    
    for (size_t i = 0; i < len; i++) {
        result[i] = input[len - 1 - i];
    }
    result[len] = '\0';
    
    return result;
}
