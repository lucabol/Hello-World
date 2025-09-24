#include <stdio.h>
#include "plugin.h"

// Voice-driven code editing simulation
// This demonstrates how voice commands could theoretically be processed
// For interactive demo, compile and run voice_demo.c

int main(void){
    int rc = 0;
    char original_message[] = "Hello world!";
    char final_message[MAX_MESSAGE_SIZE];
    
    /* Load plugins from the plugins directory */
    load_plugins_from_directory("plugins");
    
    /* Apply any registered transformers */
    apply_transformers(original_message, final_message, sizeof(final_message));
    
    /* Output the final message with plugin transformations */
    puts(final_message);
    printf("Exit code: %d\n", rc);
    
    /* Cleanup */
    cleanup_plugins();
    
    return rc;
}
