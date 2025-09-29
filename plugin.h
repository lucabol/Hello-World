#ifndef PLUGIN_H
#define PLUGIN_H

#include <stddef.h>

// Plugin interface version for compatibility checking
#define PLUGIN_API_VERSION 1

// Maximum length for plugin messages
#define MAX_MESSAGE_LEN 256

// Plugin function types
typedef int (*plugin_init_func_t)(void);
typedef char* (*plugin_transform_func_t)(const char* input);
typedef void (*plugin_cleanup_func_t)(void);
typedef int (*plugin_version_func_t)(void);

// Plugin descriptor structure
typedef struct {
    const char* name;
    const char* description;
    plugin_init_func_t init;
    plugin_transform_func_t transform;
    plugin_cleanup_func_t cleanup;
    plugin_version_func_t version;
} plugin_t;

// Plugin management functions
int load_plugins(const char* plugin_dir);
char* apply_plugins(const char* message);
void cleanup_plugins(void);

#endif // PLUGIN_H