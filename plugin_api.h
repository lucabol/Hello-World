#ifndef PLUGIN_API_H
#define PLUGIN_API_H

/**
 * Plugin API for Hello World extensibility
 * 
 * This header defines the interface that plugins must implement to extend
 * the functionality of hello.c without modifying the core program.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Plugin interface version for compatibility checking */
#define PLUGIN_API_VERSION 1

/* Plugin types */
typedef enum {
    PLUGIN_TYPE_OUTPUT_FILTER = 1,    /* Modify output text */
    PLUGIN_TYPE_POST_PROCESSOR = 2,   /* Process after main output */
    PLUGIN_TYPE_PRE_PROCESSOR = 3     /* Process before main output */
} plugin_type_t;

/* Plugin information structure */
typedef struct {
    const char* name;
    const char* version;
    const char* description;
    plugin_type_t type;
    int api_version;
} plugin_info_t;

/* Plugin function signatures */

/**
 * Get plugin information
 * This function must be implemented by all plugins
 */
typedef const plugin_info_t* (*plugin_get_info_func)(void);

/**
 * Initialize plugin
 * Called once when plugin is loaded
 * Returns 0 on success, non-zero on failure
 */
typedef int (*plugin_init_func)(void);

/**
 * Process function for output filter plugins
 * Takes input string, returns modified string (must be freed by caller)
 * If returns NULL, original string is used
 */
typedef char* (*plugin_filter_func)(const char* input);

/**
 * Process function for pre/post processor plugins  
 * Called before/after main output
 */
typedef void (*plugin_process_func)(void);

/**
 * Cleanup plugin resources
 * Called when plugin is unloaded
 */
typedef void (*plugin_cleanup_func)(void);

/* Standard plugin function names (used with dlsym) */
#define PLUGIN_GET_INFO_FUNC "plugin_get_info"
#define PLUGIN_INIT_FUNC "plugin_init"
#define PLUGIN_FILTER_FUNC "plugin_filter"
#define PLUGIN_PROCESS_FUNC "plugin_process"
#define PLUGIN_CLEANUP_FUNC "plugin_cleanup"

#ifdef __cplusplus
}
#endif

#endif /* PLUGIN_API_H */