# include <stdio.h>
#include "plugin.h"

int main(){
    char original_message[] = "Hello world!";
    char final_message[MAX_MESSAGE_SIZE];
    
    /* Load plugins from the plugins directory */
    load_plugins_from_directory("plugins");
    
    /* Apply any registered transformers */
    apply_transformers(original_message, final_message, sizeof(final_message));
    
    /* Output the final message */
    printf("%s\n", final_message);
    
    /* Cleanup */
    cleanup_plugins();
    
    return 0;
}
