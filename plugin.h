#ifndef PLUGIN_H
#define PLUGIN_H

/* Plugin interface for Hello World extensibility
 * Plugins can register transformers that modify the output message
 * 
 * ABI Contract:
 * - Plugins must implement plugin_init() function that returns plugin_info_t
 * - Plugin version must match PLUGIN_API_VERSION for compatibility
 * - Transformer functions must not write more than output_size bytes
 * - Input pointers are read-only and valid only during transformer call
 * - All functions are single-threaded (not thread-safe)
 * - Plugins are loaded once at startup and remain loaded until cleanup
 */

#define PLUGIN_API_VERSION 1
#define MAX_MESSAGE_SIZE 256
#define MAX_PLUGINS 32

/* Plugin function types */
typedef struct {
    const char* name;          /* Plugin name (must be valid C string) */
    int version;               /* Must equal PLUGIN_API_VERSION */
    const char* description;   /* Human-readable description */
} plugin_info_t;

/* Message transformer function signature
 * 
 * @param input: Read-only input message (null-terminated)
 * @param output: Output buffer to write transformed message
 * @param output_size: Maximum bytes that can be written to output (includes null terminator)
 * 
 * @return: 0 on success, -1 on error (buffer too small, invalid input, etc.)
 * 
 * REQUIREMENTS:
 * - Must null-terminate output string
 * - Must not write more than (output_size-1) characters plus null terminator
 * - Must not modify input parameter
 * - Should be reentrant (stateless) for safety
 */
typedef int (*message_transformer_t)(const char* input, char* output, size_t output_size);

/* Plugin initialization function signature
 * Must be exported by plugin as "plugin_init"
 * 
 * @return: Plugin metadata including version for ABI compatibility check
 */
typedef plugin_info_t (*plugin_init_t)(void);

/* Plugin registration functions - called by core */

/* Register a message transformer with the plugin system
 * @param name: Unique name for this transformer (for debugging/logging)
 * @param transformer: Function pointer to transformer implementation
 * @return: 0 on success, -1 if registration failed (too many plugins, etc.)
 */
int register_transformer(const char* name, message_transformer_t transformer);

/* Apply all registered transformers to input message
 * Transformers are applied in registration order (first registered, first applied)
 * @param input: Source message to transform
 * @param output: Buffer to receive final transformed message
 * @param output_size: Size of output buffer including space for null terminator
 */
void apply_transformers(const char* input, char* output, size_t output_size);

/* Plugin loader functions */

/* Load all .so files from specified directory
 * Scans directory for *.so files, loads them with dlopen(), calls plugin_init()
 * Validates ABI version compatibility before accepting plugin
 * @param dir_path: Directory path to scan for plugins
 * @return: Number of plugins loaded successfully, -1 on directory access error
 */
int load_plugins_from_directory(const char* dir_path);

/* Clean up all loaded plugins
 * Calls dlclose() on all loaded plugin handles
 * Resets internal plugin state
 */
void cleanup_plugins(void);

#endif /* PLUGIN_H */