/* Uppercase Plugin - Converts message to uppercase */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../plugin.h"

char* uppercase_plugin(const char* input) {
    if (input == NULL) {
        return NULL;
    }
    
    size_t len = strlen(input);
    char* result = (char*)malloc(len + 1);
    
    if (result == NULL) {
        return (char*)input; /* Return original on allocation failure */
    }
    
    for (size_t i = 0; i < len; i++) {
        result[i] = (char)toupper((unsigned char)input[i]);
    }
    result[len] = '\0';
    
    return result;
}
