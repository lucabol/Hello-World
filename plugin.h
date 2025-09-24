#ifndef PLUGIN_H
#define PLUGIN_H

/* Plugin interface for Hello World extensibility
 * Plugins can register transformers that modify the output message
 */

#define PLUGIN_API_VERSION 1
#define MAX_MESSAGE_SIZE 256
#define MAX_PLUGINS 32

/* Plugin function types */
typedef struct {
    const char* name;
    int version;
    const char* description;
} plugin_info_t;

/* Message transformer function signature
 * Takes input message and buffer size, returns transformed message
 * Should not modify input, should write to provided output buffer
 */
typedef int (*message_transformer_t)(const char* input, char* output, size_t output_size);

/* Plugin initialization function signature
 * Returns plugin info and registers any transformers
 */
typedef plugin_info_t (*plugin_init_t)(void);

/* Plugin registration functions - called by core */
int register_transformer(const char* name, message_transformer_t transformer);
void apply_transformers(const char* input, char* output, size_t output_size);

/* Plugin loader functions */
int load_plugins_from_directory(const char* dir_path);
void cleanup_plugins(void);

#endif /* PLUGIN_H */