/* Exclaim Plugin - Adds excitement to the message */

#include <stdlib.h>
#include <string.h>
#include "../plugin.h"

char* exclaim_plugin(const char* input) {
    if (input == NULL) {
        return NULL;
    }
    
    size_t len = strlen(input);
    /* Allocate space for original message + "!!!" */
    char* result = (char*)malloc(len + 4);
    
    if (result == NULL) {
        return (char*)input; /* Return original on allocation failure */
    }
    
    strcpy(result, input);
    strcat(result, "!!!");
    
    return result;
}
