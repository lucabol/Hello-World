/* Plugin System for hello.c
 * Simple plugin architecture allowing external developers to extend functionality
 * without modifying the core hello.c file.
 * 
 * Plugin Architecture:
 * - Plugins can transform the output message
 * - Plugins can execute before and after the main message is printed
 * - Multiple plugins can be registered and will execute in registration order
 * - Plugins are statically linked at compile time
 * 
 * Usage Example:
 *   // In your plugin file (e.g., my_plugin.c)
 *   #include "plugin.h"
 *   
 *   const char* my_transform(const char* input) {
 *       // Transform the input message
 *       static char buffer[256];
 *       snprintf(buffer, sizeof(buffer), "*** %s ***", input);
 *       return buffer;
 *   }
 *   
 *   void my_before(void) {
 *       printf("Before hook!\n");
 *   }
 *   
 *   void my_after(void) {
 *       printf("After hook!\n");
 *   }
 *   
 *   // Register your plugin (note: MyPlugin is an identifier, not a string)
 *   PLUGIN_REGISTER(MyPlugin, my_transform, my_before, my_after);
 * 
 * Compilation:
 *   gcc -DUSE_PLUGINS -o hello hello.c my_plugin.c
 */

#ifndef PLUGIN_H
#define PLUGIN_H

/* Maximum number of plugins that can be registered */
#define MAX_PLUGINS 10

/* Plugin function types */
typedef const char* (*plugin_transform_fn)(const char* input);
typedef void (*plugin_hook_fn)(void);

/* Plugin structure */
typedef struct {
    const char* name;
    plugin_transform_fn transform;
    plugin_hook_fn before;
    plugin_hook_fn after;
} plugin_t;

/* Plugin registry */
extern plugin_t plugin_registry[MAX_PLUGINS];
extern int plugin_count;

/* Register a plugin */
void plugin_register(const char* name, 
                    plugin_transform_fn transform,
                    plugin_hook_fn before,
                    plugin_hook_fn after);

/* Execute all registered plugins */
const char* plugin_execute_transforms(const char* input);
void plugin_execute_before_hooks(void);
void plugin_execute_after_hooks(void);

/* Convenience macro for plugin registration 
 * Note: Use without trailing semicolon for ISO C compliance, or wrap in extern "C" block
 */
#define PLUGIN_REGISTER(name, transform, before, after) \
    static void __plugin_init_##name(void) __attribute__((constructor)); \
    static void __plugin_init_##name(void) { \
        plugin_register(#name, transform, before, after); \
    } \
    typedef void __plugin_dummy_##name

#endif /* PLUGIN_H */
