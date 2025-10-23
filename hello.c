#include <stdio.h>
#include "hello.h"

#ifdef USE_PLUGINS
#include "plugin.h"
#define TRANSFORM_BUFFER_SIZE 1024
#endif

/* Get the greeting message */
const char* get_greeting(void) {
    static const char* greeting = "Ciao, Mondo!";
    return greeting;
}

/* Print a custom message (NULL-safe) */
void print_custom_message(const char* message) {
    if (message == NULL) {
        printf("(null message)\n");
    } else {
        printf("%s\n", message);
    }
}

#ifndef UNIT_TEST
int main(void) {
    int exit_code = 0;
    
#ifdef USE_PLUGINS
    char output_buffer[TRANSFORM_BUFFER_SIZE];
    int result;
    
    /* Execute before hooks from plugins */
    plugin_execute_before_hooks();
    
    /* Get the base message and apply any transformations */
    const char* message = get_greeting();
    result = plugin_execute_transforms(message, output_buffer, sizeof(output_buffer));
    if (result == PLUGIN_SUCCESS) {
        puts(output_buffer);
    } else {
        /* Fallback to original message on error */
        puts(message);
    }
    
    /* Execute after hooks from plugins */
    plugin_execute_after_hooks();
#else
    /* Original behavior when plugins are not enabled */
    puts(get_greeting());
#endif
    
    printf("Exit code: %d\n", exit_code);
    return exit_code;
}
#endif
