#ifndef PLUGIN_H
#define PLUGIN_H

#include <stdio.h>

/* Plugin API version - bump when interface changes */
#define PLUGIN_API_VERSION 1

/* Maximum safe sizes for security */
#define PLUGIN_MAX_NAME_LENGTH 64
#define PLUGIN_MAX_VERSION_LENGTH 16
#define PLUGIN_MAX_DESCRIPTION_LENGTH 256

/* Plugin types */
typedef enum {
    PLUGIN_TYPE_MESSAGE_TRANSFORMER = 1,
    PLUGIN_TYPE_OUTPUT_FORMATTER = 2
} plugin_type_t;

/* Plugin metadata */
typedef struct {
    const char* name;           /* Must be non-NULL and <= PLUGIN_MAX_NAME_LENGTH */
    const char* version;        /* Must be non-NULL and <= PLUGIN_MAX_VERSION_LENGTH */
    const char* description;    /* Must be non-NULL and <= PLUGIN_MAX_DESCRIPTION_LENGTH */
    plugin_type_t type;         /* Must be valid plugin type */
    int api_version;           /* Must match PLUGIN_API_VERSION */
} plugin_info_t;

/* Plugin interface functions */
typedef struct {
    /* Required: Get plugin information */
    plugin_info_t* (*get_info)(void);
    
    /* Optional: Initialize plugin (called once at startup) */
    /* Returns 0 on success, non-zero on failure */
    int (*init)(void);
    
    /* Optional: Transform message (for MESSAGE_TRANSFORMER plugins) */
    /* Returns newly allocated string that caller must free, or NULL on error */
    char* (*transform_message)(const char* input);
    
    /* Optional: Format output (for OUTPUT_FORMATTER plugins) */
    /* Should print directly to stdout */
    void (*format_output)(const char* message);
    
    /* Optional: Cleanup plugin (called once at shutdown) */
    void (*cleanup)(void);
} plugin_interface_t;

/*
 * Function that each plugin must export
 * Returns plugin interface structure
 * Must not return NULL
 */
extern plugin_interface_t* get_plugin_interface(void);

#endif /* PLUGIN_H */