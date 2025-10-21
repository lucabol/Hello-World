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
 * 
 * Compiler Support:
 * - GCC/Clang: Full automatic registration support via constructor attribute
 * - MSVC: Requires manual registration using INIT_PLUGIN() from main or init.c
 * 
 * Thread Safety:
 * - Plugin registration is NOT thread-safe
 * - Plugin application is NOT thread-safe unless plugins are written accordingly
 * - Register all plugins during startup before any concurrent access
 */

#ifndef PLUGIN_H
#define PLUGIN_H

/* Plugin function signature: takes a string, returns a transformed string */
typedef const char* (*plugin_func_t)(const char*);

/* Maximum number of plugins that can be registered (can be overridden) */
#ifndef MAX_PLUGINS
#define MAX_PLUGINS 10
#endif

/* Plugin registry - extern for cross-compilation-unit access */
extern plugin_func_t plugin_registry[MAX_PLUGINS];
extern int plugin_count;

/* Register a plugin function 
 * Returns: 0 on success, -1 if registry is full, -2 if already registered
 */
int register_plugin(plugin_func_t func);

/* Apply all registered plugins to a message */
const char* apply_plugins(const char* message);

/* Helper macro to create unique buffer names in plugins */
#define PLUGIN_BUFFER(name) plugin_buffer_##name

/* Detect compiler and define PLUGIN_CONSTRUCTOR appropriately */
#if defined(__GNUC__) || defined(__clang__)
    /* GCC and Clang support constructor attribute */
    #define PLUGIN_CONSTRUCTOR(func) \
        static void __attribute__((constructor)) func(void)
#elif defined(_MSC_VER)
    /* MSVC: constructor not supported, use manual registration */
    #define PLUGIN_CONSTRUCTOR(func) \
        static void func(void)
    #define PLUGIN_MANUAL_REGISTRATION_REQUIRED
#else
    /* Unknown compiler: assume manual registration needed */
    #define PLUGIN_CONSTRUCTOR(func) \
        static void func(void)
    #define PLUGIN_MANUAL_REGISTRATION_REQUIRED
#endif

/* Macro to define a plugin in a header file */
#define DEFINE_PLUGIN(func) \
    PLUGIN_CONSTRUCTOR(register_##func) { \
        register_plugin(func); \
    }

/* Macro to declare a plugin that will be initialized manually */
#define DECLARE_PLUGIN(func) \
    const char* func(const char*);

/* Macro to initialize a manually declared plugin */
#define INIT_PLUGIN(func) \
    register_plugin(func);

#endif /* PLUGIN_H */
