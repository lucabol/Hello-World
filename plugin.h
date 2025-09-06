#ifndef PLUGIN_H
#define PLUGIN_H

#include <stdio.h>

/* Plugin API version - bump when interface changes */
#define PLUGIN_API_VERSION 1

/* Plugin types */
typedef enum {
    PLUGIN_TYPE_MESSAGE_TRANSFORMER = 1,
    PLUGIN_TYPE_OUTPUT_FORMATTER = 2
} plugin_type_t;

/* Plugin metadata */
typedef struct {
    const char* name;
    const char* version;
    const char* description;
    plugin_type_t type;
    int api_version;
} plugin_info_t;

/* Plugin interface functions */
typedef struct {
    /* Required: Get plugin information */
    plugin_info_t* (*get_info)(void);
    
    /* Optional: Initialize plugin (called once at startup) */
    int (*init)(void);
    
    /* Optional: Transform message (for MESSAGE_TRANSFORMER plugins) */
    char* (*transform_message)(const char* input);
    
    /* Optional: Format output (for OUTPUT_FORMATTER plugins) */
    void (*format_output)(const char* message);
    
    /* Optional: Cleanup plugin (called once at shutdown) */
    void (*cleanup)(void);
} plugin_interface_t;

/* Function that each plugin must export */
extern plugin_interface_t* get_plugin_interface(void);

#endif /* PLUGIN_H */