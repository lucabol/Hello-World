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
 * 
 * IMPORTANT: With stack allocation (default), 2x this size is allocated on the stack.
 * For embedded/small-stack environments or large buffers (>4KB), use heap allocation
 * by compiling with -DPLUGIN_USE_HEAP to avoid stack overflow.
 */
#ifndef PLUGIN_BUFFER_SIZE
#define PLUGIN_BUFFER_SIZE 1024
#endif

#if PLUGIN_BUFFER_SIZE > 4096
#warning "PLUGIN_BUFFER_SIZE > 4KB: Consider using -DPLUGIN_USE_HEAP to avoid stack overflow"
#endif

/* 
 * PORTABILITY AND LIMITATIONS:
 * 
 * - COMPILER SUPPORT: This plugin system uses GCC/Clang __attribute__((constructor))
 *   for automatic registration. This is not portable to MSVC or other non-GCC-compatible
 *   compilers. If using unsupported compilers, plugins must use explicit registration.
 * 
 * - LTO/LINKER STRIPPING: When building as a static library or with link-time optimization,
 *   linkers may strip unused constructor functions. To prevent this:
 *   a) Use explicit registration (call plugin_register from main), OR
 *   b) Add linker flags to preserve constructors (e.g., --whole-archive on GNU ld), OR
 *   c) Reference plugin symbols explicitly in main to prevent stripping
 * 
 * - REGISTRATION ORDER: Constructor execution order across translation units is not
 *   guaranteed by the C standard. Plugin registration order may vary depending on link
 *   order. Do not rely on specific plugin execution order for correctness.
 * 
 * - THREAD SAFETY: The plugin system is NOT thread-safe during registration.
 *   **CRITICAL**: All plugin registration MUST complete before any threads are spawned
 *   or before the first call to plugin_execute_* functions. Do NOT call plugin_register()
 *   concurrently from multiple threads - this will cause race conditions and registry
 *   corruption. Transform functions should be reentrant and avoid mutable global state
 *   unless synchronized externally.
 * 
 * - REENTRANCY: Transform functions receive independent output buffers and should
 *   not rely on static/global mutable state to ensure correct chaining behavior.
 * 
 * - MEMORY ALLOCATION: By default, double-buffering uses stack allocation (2x PLUGIN_BUFFER_SIZE).
 *   For embedded/small-stack environments or large buffers, compile with -DPLUGIN_USE_HEAP
 *   to use heap allocation instead. See plugin.c for details.
 */

/* Plugin function types with clear naming
 * 
 * Transform function signature:
 *   int transform(const char* input, char* output, size_t output_size)
 * 
 * Parameters:
 *   - input: The input message to transform (null-terminated, read-only)
 *   - output: Buffer to write the transformed message
 *   - output_size: Size of the output buffer in bytes (including space for null terminator)
 * 
 * Returns:
 *   - PLUGIN_SUCCESS (0) on success
 *   - PLUGIN_ERROR_BUFFER_TOO_SMALL if output buffer is insufficient
 *   - PLUGIN_ERROR_INVALID_INPUT if input is malformed
 *   - PLUGIN_ERROR_INTERNAL for other plugin-specific errors
 * 
 * GUARANTEES AND SEMANTICS:
 * 
 * 1. Null-termination: On PLUGIN_SUCCESS, the output MUST be a valid null-terminated
 *    string that fits within output_size bytes (including the null terminator).
 *    Plugins MUST ensure output[output_size-1] is never written past.
 * 
 * 2. Buffer overflow: If the transformed output would require more than output_size
 *    bytes (including null terminator), the plugin MUST return PLUGIN_ERROR_BUFFER_TOO_SMALL
 *    and leave the output buffer in an undefined state. The caller will not use the output.
 * 
 * 3. No retry semantics: PLUGIN_ERROR_BUFFER_TOO_SMALL is treated as a fatal error.
 *    The transform chain stops immediately and hello.c falls back to the original message.
 *    Callers should not retry with a larger buffer - configure PLUGIN_BUFFER_SIZE at
 *    compile time if needed.
 * 
 * 4. Error propagation: If a transform returns any non-PLUGIN_SUCCESS code:
 *    - The transform chain stops immediately
 *    - The error code is returned to hello.c
 *    - hello.c will print the original untransformed message
 *    - After hooks will still execute (before hooks already executed)
 * 
 * 5. Buffer contents on error: If a transform returns an error, the contents of the
 *    output buffer are undefined and will not be used. Plugins need not clear or
 *    initialize the buffer on error paths.
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
 * 
 * PORTABILITY NOTES:
 * 
 * This macro uses __attribute__((constructor)) which is supported by GCC and Clang
 * but NOT by MSVC or other C compilers. The constructor attribute causes the
 * registration function to run automatically before main() is called.
 * 
 * REGISTRATION ORDERING:
 * Constructor execution order across different translation units (source files) is
 * not guaranteed by the C standard and depends on link order. Do not rely on specific
 * plugin execution order for correctness. If deterministic ordering is required,
 * use explicit registration (see MSVC example in PLUGIN_GUIDE.md).
 * 
 * FOR MSVC/OTHER COMPILERS:
 * Do not use this macro. Instead, call plugin_register() explicitly from a plugin_init()
 * function and call plugin_init() from main() before using plugins. See PLUGIN_GUIDE.md
 * for a complete example.
 * 
 * The compile-time error below ensures you don't accidentally try to use auto-registration
 * on unsupported compilers. To suppress this error and use manual registration, compile
 * with -DPLUGIN_MANUAL_REGISTRATION.
 */
#if defined(__GNUC__) || defined(__clang__)
#define PLUGIN_REGISTER(name, transform, before, after) \
    static void __plugin_init_##name(void) __attribute__((constructor)); \
    static void __plugin_init_##name(void) { \
        plugin_register(#name, transform, before, after); \
    } \
    typedef void __plugin_dummy_##name
#elif defined(PLUGIN_MANUAL_REGISTRATION)
/* Manual registration mode - PLUGIN_REGISTER is a no-op */
#define PLUGIN_REGISTER(name, transform, before, after) \
    typedef void __plugin_dummy_##name
#else
#error "Plugin auto-registration (PLUGIN_REGISTER macro) requires GCC or Clang. For MSVC or other compilers, use explicit plugin_register() calls and compile with -DPLUGIN_MANUAL_REGISTRATION. See PLUGIN_GUIDE.md for examples."
#endif

#endif /* PLUGIN_H */
