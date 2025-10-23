# include <stdio.h>

#ifdef USE_PLUGINS
#include "plugin.h"
#define TRANSFORM_BUFFER_SIZE 1024
#endif

int main(){
#ifdef USE_PLUGINS
    char output_buffer[TRANSFORM_BUFFER_SIZE];
    
    /* Execute before hooks from plugins */
    plugin_execute_before_hooks();
    
    /* Get the base message and apply any transformations */
    const char* message = "Hello world!";
    if (plugin_execute_transforms(message, output_buffer, sizeof(output_buffer)) == 0) {
        printf("%s", output_buffer);
    } else {
        /* Fallback to original message on error */
        printf("%s", message);
    }
    
    /* Execute after hooks from plugins */
    plugin_execute_after_hooks();
#else
    /* Original behavior when plugins are not enabled */
    printf("Hello world!");
#endif
}
