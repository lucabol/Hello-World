#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include "plugin_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Plugin structure for internal use */
typedef struct {
    void* handle;  /* dlopen handle */
    const plugin_info_t* info;
    
    /* Plugin functions */
    plugin_get_info_func get_info;
    plugin_init_func init;
    plugin_filter_func filter;
    plugin_process_func process;
    plugin_cleanup_func cleanup;
} plugin_t;

/**
 * Initialize plugin loader and scan for plugins
 * @param plugin_dir Directory to scan for plugins (NULL for default "./plugins")
 * @return Number of plugins loaded, or -1 on error
 */
int plugin_loader_init(const char* plugin_dir);

/**
 * Cleanup and unload all plugins
 */
void plugin_loader_cleanup(void);

/**
 * Apply all output filter plugins to input string
 * @param input Input string to filter
 * @return Filtered string (caller must free), or NULL if no filters applied
 */
char* plugin_apply_filters(const char* input);

/**
 * Run all preprocessor plugins
 */
void plugin_run_preprocessors(void);

/**
 * Run all postprocessor plugins
 */
void plugin_run_postprocessors(void);

/**
 * Get number of loaded plugins
 * @return Number of loaded plugins
 */
int plugin_get_count(void);

/**
 * Get plugin info by index
 * @param index Plugin index (0 to plugin_get_count() - 1)
 * @return Plugin info, or NULL if index invalid
 */
const plugin_info_t* plugin_get_info_by_index(int index);

#ifdef __cplusplus
}
#endif

#endif /* PLUGIN_LOADER_H */