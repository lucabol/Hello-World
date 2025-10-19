#include <stdio.h>
#include <stdlib.h>

#ifdef USE_PLUGINS
#include "plugin.h"
#endif

int main(){
    const char* message = "Hello world!";
    
#ifdef USE_PLUGINS
    char* transformed = apply_plugins(message);
    printf("%s", transformed);
    
    /* Free transformed message if it was allocated by plugins */
    if (transformed != message) {
        free(transformed);
    }
#else
    printf("%s", message);
#endif
    
    return 0;
}
