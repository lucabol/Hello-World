#ifndef PLUGIN_H
#define PLUGIN_H

/**
 * Plugin interface for hello.c extensibility
 * 
 * Plugins can implement these functions to extend functionality:
 * - plugin_init(): Called when plugin is loaded
 * - plugin_transform(): Transform the output message
 * - plugin_cleanup(): Called before program exits
 */

typedef struct {
    const char* name;
    const char* version;
    void (*init)(void);
    char* (*transform)(const char* input);
    void (*cleanup)(void);
} plugin_t;

/* Plugin registration function that each plugin must implement */
plugin_t* plugin_register(void);

#endif /* PLUGIN_H */