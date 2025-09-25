#include <stdio.h>

// Voice-driven code editing simulation
// This demonstrates how voice commands could theoretically be processed
// For interactive demo, compile and run voice_demo.c

// Plugin system integration (optional compilation)
#ifdef ENABLE_PLUGINS
#include "plugin.h"
#include "sample_plugins.h"
#endif

int main(void){
    int rc = 0;
    
#ifdef ENABLE_PLUGINS
    // Initialize plugin system if enabled
    register_sample_plugins();
    plugin_load_all();
    
    // Run pre-processors
    plugin_run_pre_processors();
#endif
    
    // Default behavior - maintain compatibility
    const char* message = "Hello world!";
    
#ifdef ENABLE_PLUGINS
    // Apply plugin transformations
    message = plugin_transform_output(message);
#endif
    
    puts(message);
    printf("Exit code: %d\n", rc);
    
#ifdef ENABLE_PLUGINS
    // Run post-processors
    plugin_run_post_processors();
    
    // Cleanup plugins
    plugin_cleanup_all();
#endif
    
    return rc;
}
