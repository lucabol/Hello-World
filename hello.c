#include <stdio.h>

#ifdef ENABLE_PLUGINS
#include <stdlib.h>
#include <string.h>
#include "plugin_loader.h"

/* Helper function to duplicate string */
static char* duplicate_string(const char* str) {
    size_t len = strlen(str);
    char* dup = malloc(len + 1);
    if (dup) {
        memcpy(dup, str, len + 1);
    }
    return dup;
}
#endif

int main() {
    const char* original_message = "Hello world!";
    
#ifdef ENABLE_PLUGINS
    char* final_message = NULL;
    
    /* Initialize plugin system */
    plugin_loader_init(NULL);
    
    /* Run preprocessors */
    plugin_run_preprocessors();
    
    /* Apply output filters */
    final_message = plugin_apply_filters(original_message);
    if (!final_message) {
        final_message = duplicate_string(original_message);
    }
    
    /* Output the (possibly modified) message */
    printf("%s", final_message);
    
    /* Run postprocessors */
    plugin_run_postprocessors();
    
    /* Cleanup */
    free(final_message);
    plugin_loader_cleanup();
#else
    /* Plugin system disabled - use original behavior */
    printf("%s", original_message);
#endif
    
    return 0;
}
