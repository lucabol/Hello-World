#ifndef PLUGIN_H
#define PLUGIN_H

/**
 * Plugin System for Hello World C Program
 * 
 * This header defines the plugin architecture that allows external developers
 * to extend hello.c functionality without modifying the core file directly.
 * 
 * Plugin Types:
 * - OUTPUT_TRANSFORMER: Modify the output message
 * - PRE_PROCESSOR: Execute before main functionality
 * - POST_PROCESSOR: Execute after main functionality
 */

#include <stdio.h>

// Plugin types
typedef enum {
    PLUGIN_TYPE_OUTPUT_TRANSFORMER = 1,
    PLUGIN_TYPE_PRE_PROCESSOR = 2,
    PLUGIN_TYPE_POST_PROCESSOR = 3
} plugin_type_t;

// Plugin function signatures
typedef int (*plugin_init_func_t)(void);
typedef void (*plugin_cleanup_func_t)(void);
typedef const char* (*output_transformer_func_t)(const char* original);
typedef int (*processor_func_t)(void);

// Plugin descriptor structure
typedef struct {
    const char* name;
    const char* version;
    const char* author;
    plugin_type_t type;
    
    // Function pointers (only relevant ones need to be set)
    plugin_init_func_t init;
    plugin_cleanup_func_t cleanup;
    output_transformer_func_t transform_output;
    processor_func_t process;
} plugin_t;

// Maximum number of plugins
#define MAX_PLUGINS 10

// Plugin registry functions
int plugin_register(const plugin_t* plugin);
int plugin_load_all(void);
void plugin_cleanup_all(void);

// Plugin execution functions  
const char* plugin_transform_output(const char* original);
int plugin_run_pre_processors(void);
int plugin_run_post_processors(void);

// Utility functions for plugin developers
void plugin_log(const char* message);

#endif /* PLUGIN_H */