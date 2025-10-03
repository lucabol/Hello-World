/* Example Plugin: ROT13 Cipher
 * 
 * This plugin demonstrates a more advanced transformation - the ROT13 cipher.
 * ROT13 is a simple letter substitution cipher that replaces a letter with the 
 * letter 13 positions after it in the alphabet.
 * 
 * Example: "Hello world!" -> "Uryyb jbeyq!"
 * 
 * To use this plugin:
 *   gcc -o hello hello.c plugin.c plugins/rot13_plugin.c
 * 
 * Educational value: Shows how to implement character-by-character transformations
 * and demonstrates conditional logic within a plugin.
 */

#include "../plugin.h"
#include <string.h>
#include <ctype.h>

/* Static buffer for the transformed message */
static char rot13_buffer[256];

/* ROT13 transformation function */
static char rot13_char(char c) {
    if (c >= 'a' && c <= 'z') {
        return (char)('a' + (c - 'a' + 13) % 26);
    } else if (c >= 'A' && c <= 'Z') {
        return (char)('A' + (c - 'A' + 13) % 26);
    } else {
        /* Non-alphabetic characters are not transformed */
        return c;
    }
}

/* Transform message using ROT13 */
static char* rot13_transform(const char* input) {
    size_t i;
    size_t len = strlen(input);
    
    if (len >= sizeof(rot13_buffer)) {
        len = sizeof(rot13_buffer) - 1;
    }
    
    for (i = 0; i < len; i++) {
        rot13_buffer[i] = rot13_char(input[i]);
    }
    rot13_buffer[len] = '\0';
    
    return rot13_buffer;
}

/* Define the plugin */
static plugin_interface rot13_plugin = {
    .process_message = rot13_transform,
    .plugin_name = "ROT13Plugin",
    .plugin_version = "1.0.0"
};

/* Register the plugin (automatically called at program startup) */
REGISTER_PLUGIN(rot13_plugin);
