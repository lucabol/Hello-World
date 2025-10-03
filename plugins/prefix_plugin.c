/* Example Plugin: Prefix Transformer
 * 
 * This plugin demonstrates how to create a plugin that adds a prefix to messages.
 * It adds "*** " before the greeting message.
 * 
 * To use this plugin along with others:
 *   gcc -o hello hello.c plugin.c plugins/prefix_plugin.c plugins/example_plugin.c
 * 
 * Plugins are applied in the order they are compiled.
 */

#include "../plugin.h"
#include <stdio.h>
#include <string.h>

/* Static buffer for the transformed message */
static char prefix_buffer[256];

/* Add prefix to message */
static char* prefix_transform(const char* input) {
    snprintf(prefix_buffer, sizeof(prefix_buffer), "*** %s", input);
    return prefix_buffer;
}

/* Define the plugin */
static plugin_interface prefix_plugin = {
    .process_message = prefix_transform,
    .plugin_name = "PrefixPlugin",
    .plugin_version = "1.0.0"
};

/* Register the plugin (automatically called at program startup) */
REGISTER_PLUGIN(prefix_plugin);
