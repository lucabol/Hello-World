# include <stdio.h>

#ifdef USE_PLUGINS
#include "plugin.h"
#endif

int main(){
#ifdef USE_PLUGINS
    /* Execute before hooks from plugins */
    plugin_execute_before_hooks();
    
    /* Get the base message and apply any transformations */
    const char* message = "Hello world!";
    message = plugin_execute_transforms(message);
    printf("%s", message);
    
    /* Execute after hooks from plugins */
    plugin_execute_after_hooks();
#else
    /* Original behavior when plugins are not enabled */
    printf("Hello world!");
#endif
}
