#include <stdio.h>
#include "plugin.h"

int main(void){
    const char* message = "Ciao, Mondo!";
    char output[PLUGIN_MAX_MESSAGE_LEN];
    
    /* Apply plugins if any are registered */
    int result = apply_plugins(message, output, sizeof(output));
    
    if (result >= 0) {
        puts(output);
    } else {
        /* Fallback to original message if plugin system fails */
        puts(message);
    }
    puts("Exit code: 0");
    
    return 0;
}
