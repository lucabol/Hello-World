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
 *   int my_transform(const char* input, char* output, size_t output_size) {
 *       // Transform the input message
 *       int result = snprintf(output, output_size, "*** %s ***", input);
 *       if (result < 0 || (size_t)result >= output_size) {
 *           return PLUGIN_ERROR_BUFFER_TOO_SMALL;
 *       }
 *       return PLUGIN_SUCCESS;
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

#include <stddef.h>  /* For size_t */

/* Plugin API version for compatibility checking */
#define HELLO_PLUGIN_API_VERSION 1

/* Plugin error codes */
#define PLUGIN_SUCCESS 0
#define PLUGIN_ERROR_BUFFER_TOO_SMALL -1
#define PLUGIN_ERROR_INVALID_INPUT -2
#define PLUGIN_ERROR_INTERNAL -3
#define PLUGIN_ERROR_MAX_PLUGINS_EXCEEDED -4

/* Maximum number of plugins that can be registered 
 * Can be overridden at compile time with -DMAX_PLUGINS=N
 */
#ifndef MAX_PLUGINS
#define MAX_PLUGINS 10
#endif

/* Plugin buffer size for transform chaining 
 * Can be overridden at compile time with -DPLUGIN_BUFFER_SIZE=N
 */
#ifndef PLUGIN_BUFFER_SIZE
#define PLUGIN_BUFFER_SIZE 1024
#endif

/* 
 * PORTABILITY AND LIMITATIONS:
 * 
 * - This plugin system uses GCC/Clang __attribute__((constructor)) for automatic
 *   registration. This is not portable to MSVC or other non-GCC-compatible compilers.
 *   If using unsupported compilers, plugins must use explicit registration (see below).
 * 
 * - Constructor execution order across translation units is not guaranteed by the C
 *   standard. Plugin registration order may vary depending on link order. Do not
 *   rely on specific plugin execution order for correctness.
 * 
 * - THREAD SAFETY: The plugin system is NOT thread-safe. All plugin registration
 *   must complete before any plugin execution. Transform functions should be reentrant
 *   and avoid mutable global state unless synchronized externally.
 * 
 * - REENTRANCY: Transform functions receive independent output buffers and should
 *   not rely on static/global mutable state to ensure correct chaining behavior.
 */

/* Plugin function types with clear naming
 * 
 * Transform function signature:
 *   int transform(const char* input, char* output, size_t output_size)
 * 
 * Parameters:
 *   - input: The input message to transform (null-terminated, read-only)
 *   - output: Buffer to write the transformed message (null-terminated)
 *   - output_size: Size of the output buffer in bytes
 * 
 * Returns:
 *   - PLUGIN_SUCCESS (0) on success
 *   - PLUGIN_ERROR_BUFFER_TOO_SMALL if output buffer is insufficient
 *   - PLUGIN_ERROR_INVALID_INPUT if input is malformed
 *   - PLUGIN_ERROR_INTERNAL for other plugin-specific errors
 * 
 * The transform function must write a null-terminated string to output
 * and ensure it does not exceed output_size bytes (including null terminator).
 * If the output would exceed the buffer, the function should return PLUGIN_ERROR_BUFFER_TOO_SMALL.
 */
typedef int (*plugin_transform_fn)(const char* input, char* output, size_t output_size);
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

/* Register a plugin 
 * This function can be called explicitly from plugin code for portability
 * to compilers that don't support constructor attributes.
 * 
 * Returns:
 *   - PLUGIN_SUCCESS on successful registration
 *   - PLUGIN_ERROR_MAX_PLUGINS_EXCEEDED if registry is full
 */
int plugin_register(const char* name, 
                   plugin_transform_fn transform,
                   plugin_hook_fn before,
                   plugin_hook_fn after);

/* Get the current plugin count (useful for testing overflow) */
int plugin_get_count(void);

/* Execute all registered plugins 
 * 
 * Behavior on error:
 * - If a transform returns non-zero, the error is logged and the last successful
 *   transform output is used. The error code is propagated to the caller.
 * - Hooks (before/after) have no error handling - they should not fail.
 * - If input is too long (>= PLUGIN_BUFFER_SIZE), returns PLUGIN_ERROR_INVALID_INPUT
 * - If final output is truncated, a warning is printed but PLUGIN_SUCCESS is returned
 * 
 * Returns:
 *   - PLUGIN_SUCCESS if all transforms succeeded
 *   - PLUGIN_ERROR_INVALID_INPUT if input validation fails
 *   - First non-zero error code from a failing transform
 */
int plugin_execute_transforms(const char* input, char* output, size_t output_size);
void plugin_execute_before_hooks(void);
void plugin_execute_after_hooks(void);

/* Convenience macro for plugin registration 
 * Note: Uses GCC/Clang constructor attribute. Not portable to MSVC.
 * For non-GCC compilers, call plugin_register() explicitly.
 */
#if defined(__GNUC__) || defined(__clang__)
#define PLUGIN_REGISTER(name, transform, before, after) \
    static void __plugin_init_##name(void) __attribute__((constructor)); \
    static void __plugin_init_##name(void) { \
        plugin_register(#name, transform, before, after); \
    } \
    typedef void __plugin_dummy_##name
#else
#error "Plugin auto-registration requires GCC or Clang. Use explicit plugin_register() calls or define PLUGIN_MANUAL_REGISTRATION."
#endif

#endif /* PLUGIN_H */
