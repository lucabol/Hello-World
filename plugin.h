/* Plugin System for Hello World
 * Provides a simple interface for extending hello.c functionality
 * without modifying the core file directly.
 * 
 * Plugin developers can create transformation functions that modify
 * the greeting message before it is printed.
 * 
 * Usage - Option 1 (Header-only plugins):
 * 1. Create a .h file with your plugin function and DEFINE_PLUGIN macro
 * 2. Include the plugin header in hello.c or your main file
 * 
 * Usage - Option 2 (Separate compilation):
 * 1. Define plugin function in a .c file
 * 2. Declare it with DECLARE_PLUGIN in a .h file
 * 3. Call INIT_PLUGIN in main() before using plugins
 */

#ifndef PLUGIN_H
#define PLUGIN_H

/* Plugin function signature: takes a string, returns a transformed string */
typedef const char* (*plugin_func_t)(const char*);

/* Maximum number of plugins that can be registered */
#define MAX_PLUGINS 10

/* Plugin registry - extern for cross-compilation-unit access */
extern plugin_func_t plugin_registry[MAX_PLUGINS];
extern int plugin_count;

/* Register a plugin function */
void register_plugin(plugin_func_t func);

/* Apply all registered plugins to a message */
const char* apply_plugins(const char* message);

/* Macro to define a plugin in a header file */
#define DEFINE_PLUGIN(func) \
    static void __attribute__((constructor)) register_##func(void) { \
        register_plugin(func); \
    }

/* Macro to declare a plugin that will be initialized manually */
#define DECLARE_PLUGIN(func) \
    const char* func(const char*);

/* Macro to initialize a manually declared plugin */
#define INIT_PLUGIN(func) \
    register_plugin(func);

#endif /* PLUGIN_H */
