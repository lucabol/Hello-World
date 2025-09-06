#ifndef PLUGIN_H
#define PLUGIN_H

/* Plugin interface for extending hello.c functionality */
typedef struct {
    const char* name;
    char* (*transform_message)(const char* input);
    void (*pre_main_hook)(void);
    void (*post_main_hook)(void);
} plugin_t;

/* Plugin registry functions */
void register_plugin(plugin_t* plugin);
void execute_pre_hooks(void);
char* apply_transformations(const char* message);
void execute_post_hooks(void);

#endif /* PLUGIN_H */