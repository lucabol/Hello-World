# include <stdio.h>

/* Plugin System - Inline Implementation */
typedef const char* (*plugin_function)(const char* input);

typedef struct {
    const char* name;
    const char* description;
    plugin_function transform;
} plugin_t;

/* Apply plugins to input string */
static const char* apply_plugins(const char* input, const plugin_t* plugins, int plugin_count) {
    const char* current = input;
    int i;
    
    if (plugins == NULL || plugin_count == 0) {
        return input;
    }
    
    for (i = 0; i < plugin_count; i++) {
        if (plugins[i].transform != NULL) {
            current = plugins[i].transform(current);
        }
    }
    
    return current;
}

/* Default greeting message */
static const char* greeting = "Hello world!";

/* Plugin array - add plugins here to extend functionality
 * When no plugins are defined, initialize with a placeholder to avoid empty array
 */
static plugin_t plugins[] = {
    {NULL, NULL, NULL}  /* Placeholder - will be ignored by plugin loader */
};
static int plugin_count = 0;  /* Set to 0 to indicate no active plugins */

int main(){
    /* Apply plugins to greeting (if any are registered) */
    const char* final_message = apply_plugins(greeting, plugins, plugin_count);
    
    printf("%s\n", final_message);
}
