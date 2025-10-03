/* Example Plugin: Uppercase Transformer
 * 
 * This plugin demonstrates how to create a plugin for the Hello World program.
 * It transforms the greeting message to uppercase.
 * 
 * To use this plugin:
 *   gcc -o hello hello.c plugin.c plugins/example_plugin.c
 * 
 * To build without this plugin:
 *   gcc -o hello hello.c plugin.c
 */

#include "../plugin.h"
#include <ctype.h>
#include <string.h>

/* Static buffer for the transformed message */
static char uppercase_buffer[256];

/* Transform message to uppercase */
static char* uppercase_transform(const char* input) {
    size_t i;
    size_t len = strlen(input);
    
    if (len >= sizeof(uppercase_buffer)) {
        len = sizeof(uppercase_buffer) - 1;
    }
    
    for (i = 0; i < len; i++) {
        uppercase_buffer[i] = (char)toupper((unsigned char)input[i]);
    }
    uppercase_buffer[len] = '\0';
    
    return uppercase_buffer;
}

/* Define the plugin */
static plugin_interface uppercase_plugin = {
    .process_message = uppercase_transform,
    .plugin_name = "UppercasePlugin",
    .plugin_version = "1.0.0"
};

/* Register the plugin (automatically called at program startup) */
REGISTER_PLUGIN(uppercase_plugin);
