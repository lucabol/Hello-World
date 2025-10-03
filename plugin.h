/* Plugin System for Hello World
 * 
 * This header defines the plugin interface for extending hello.c functionality.
 * External developers can create plugins that transform or modify the greeting
 * message without modifying hello.c directly.
 * 
 * PORTABILITY NOTE:
 * This plugin system uses GCC/Clang constructor attributes for auto-registration.
 * Supported compilers: GCC 4.7+, Clang 3.3+
 * Not supported: MSVC, some embedded toolchains
 * 
 * MEMORY MANAGEMENT CONTRACT:
 * - Plugins MUST return pointers to static/global storage or thread-local storage
 * - Plugins MUST NOT return pointers to stack variables (local arrays)
 * - Plugins MUST NOT use malloc() without proper cleanup strategy
 * - The returned pointer must remain valid for the lifetime of the program
 * - Caller (apply_plugins) does NOT take ownership and will NOT free returned pointers
 * 
 * THREAD SAFETY:
 * - Plugin registration occurs at program startup (before main)
 * - apply_plugins() is NOT thread-safe if plugins use shared mutable state
 * - Plugins using static buffers are not thread-safe
 * - For thread-safe plugins, use thread-local storage (__thread or _Thread_local)
 * 
 * Plugin Interface:
 * - process_message: Transform the greeting message
 * - plugin_name: Name of the plugin for identification
 * - plugin_version: Version string for the plugin
 * 
 * Usage:
 * 1. Create a plugin by implementing the plugin_interface struct
 * 2. Register the plugin using REGISTER_PLUGIN macro
 * 3. Compile your plugin with hello.c
 * 
 * Example Plugin:
 * 
 *   #include "plugin.h"
 *   
 *   static char* my_transform(const char* input) {
 *       static char buffer[256];
 *       snprintf(buffer, sizeof(buffer), "[Modified] %s", input);
 *       return buffer;
 *   }
 *   
 *   static plugin_interface my_plugin = {
 *       .process_message = my_transform,
 *       .plugin_name = "MyPlugin",
 *       .plugin_version = "1.0"
 *   };
 *   
 *   REGISTER_PLUGIN(my_plugin);
 */

#ifndef PLUGIN_H
#define PLUGIN_H

/* Check compiler support for constructor attributes */
#if defined(__GNUC__) || defined(__clang__)
    /* GCC and Clang support __attribute__((constructor)) */
    #define PLUGIN_CONSTRUCTOR_SUPPORTED 1
#else
    /* Unsupported compiler */
    #error "Plugin system requires GCC 4.7+ or Clang 3.3+ for constructor attribute support. MSVC and other compilers are not currently supported."
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Plugin interface structure
 * 
 * MEMORY CONTRACT:
 * - process_message receives a const char* input (plugin must not modify it)
 * - process_message returns char* pointing to static/global/thread-local storage
 * - Returned pointer must remain valid for the program's lifetime
 * - Return NULL to skip transformation (pass through to next plugin)
 * - Caller does NOT take ownership and will NOT free the returned pointer
 * 
 * ERROR HANDLING:
 * - If process_message returns NULL, the transformation is skipped
 * - The input message is passed unchanged to the next plugin (or returned as final result)
 * - Plugins should NOT call exit() or abort() on errors
 * - Plugins should print errors to stderr and return NULL to indicate failure
 */
typedef struct {
    /* Transform the input message and return the result.
     * 
     * @param input - The message to transform (must not be modified)
     * @return Pointer to static/global storage containing transformed message,
     *         or NULL to skip transformation (pass through)
     */
    char* (*process_message)(const char* input);
    
    /* Name of the plugin (for debugging and diagnostics) */
    const char* plugin_name;
    
    /* Version of the plugin (semantic versioning recommended) */
    const char* plugin_version;
} plugin_interface;

/* Maximum number of plugins that can be registered */
#define MAX_PLUGINS 16

/* Plugin registry - array of plugin pointers */
extern plugin_interface* plugin_registry[MAX_PLUGINS];
extern int plugin_count;

/* Register a plugin - to be called from plugin implementations
 * This function is typically called automatically by REGISTER_PLUGIN macro.
 * Can also be called manually during program initialization if needed.
 * 
 * @param plugin - Pointer to plugin_interface structure (must remain valid)
 */
void register_plugin(plugin_interface* plugin);

/* Macro to simplify plugin registration at program initialization
 * 
 * This macro uses GCC/Clang __attribute__((constructor)) to automatically
 * register plugins before main() is called.
 * 
 * PLUGIN ORDERING:
 * - Plugins are registered in link order (order specified in compilation command)
 * - The order is generally deterministic for a given link command
 * - To guarantee specific ordering, compile plugins in desired order
 * - Example: gcc hello.c plugin.c plugin1.c plugin2.c plugin3.c
 *   will register plugins in order: plugin1, plugin2, plugin3
 * 
 * Usage: REGISTER_PLUGIN(my_plugin);
 * Note: The semicolon after the macro is required and will be consumed.
 */
#define REGISTER_PLUGIN(plugin_var) \
    static void __attribute__((constructor)) register_##plugin_var(void) { \
        register_plugin(&plugin_var); \
    } \
    typedef int register_plugin_dummy_##plugin_var

/* Apply all registered plugins to a message
 * 
 * Applies each registered plugin sequentially in registration order.
 * If a plugin returns NULL, it is skipped and the previous result is passed
 * to the next plugin. If all plugins return NULL or no plugins are registered,
 * returns the original input message.
 * 
 * MEMORY CONTRACT:
 * - Takes a const char* input message (will not be modified)
 * - Returns const char* to final transformed message
 * - Returned pointer points to either:
 *   a) The original input message (if no plugins transformed it)
 *   b) Static/global storage from the last plugin that transformed it
 * - Caller must NOT free the returned pointer
 * - Returned pointer remains valid for the program's lifetime
 * 
 * THREAD SAFETY:
 * - NOT thread-safe if plugins use shared mutable state
 * - Safe to call from multiple threads if all plugins are read-only or use thread-local storage
 * 
 * ERROR HANDLING:
 * - If a plugin returns NULL, the transformation is skipped (not treated as error)
 * - No way for plugins to signal hard errors; they should print to stderr and return NULL
 * 
 * @param message - Original message to transform
 * @return Final transformed message (or original if no transformations applied)
 */
const char* apply_plugins(const char* message);

#ifdef __cplusplus
}
#endif

#endif /* PLUGIN_H */
