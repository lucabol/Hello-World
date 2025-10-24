# include <stdio.h>
#include "plugin.h"

int main(){
    const char* message = "Hello world!";
    char output[PLUGIN_MAX_MESSAGE_LEN];
    
    /* Apply plugins if any are registered */
    int result = apply_plugins(message, output, sizeof(output));
    
    if (result >= 0) {
        printf("%s", output);
    } else {
        /* Fallback to original message if plugin system fails */
        printf("%s", message);
    }
    
    return 0;
}
