# include <stdio.h>

#ifdef ENABLE_PLUGINS
#include "plugin.h"
#endif

int main(){
#ifdef ENABLE_PLUGINS
    /* Initialize plugins and apply transformations */
    char output[256];
    const char* message = "Hello world!";
    
    /* Apply plugins if any are registered */
    if (plugin_count() > 0) {
        plugin_apply_all(message, output, sizeof(output));
        printf("%s", output);
    } else {
        printf("%s", message);
    }
#else
    /* Default behavior: simple output without plugin support */
    printf("Hello world!");
#endif
}
