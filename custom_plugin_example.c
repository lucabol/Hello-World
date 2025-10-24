/* Custom Plugin Example
 * SPDX-License-Identifier: MIT
 * 
 * This example demonstrates how an external developer can create
 * a custom plugin without modifying hello.c or any core files.
 * 
 * This plugin implements a simple ROT13 cipher transformation.
 */

#include <stdio.h>
#include "plugin.h"

/* ROT13 Plugin - Simple letter rotation cipher
 * Rotates each letter 13 positions in the alphabet
 * Non-alphabetic characters are left unchanged
 */
static int rot13_transform(const char* input, char* output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return -1;
    }
    
    size_t i;
    size_t len = 0;
    
    for (i = 0; input[i] != '\0' && i < output_size - 1; i++) {
        char c = input[i];
        
        if (c >= 'a' && c <= 'z') {
            output[i] = (char)('a' + ((c - 'a' + 13) % 26));
        } else if (c >= 'A' && c <= 'Z') {
            output[i] = (char)('A' + ((c - 'A' + 13) % 26));
        } else {
            output[i] = c;
        }
        len++;
    }
    
    output[len] = '\0';
    return 0;
}

/* Plugin definition */
plugin_t rot13_plugin = {
    .name = "rot13",
    .transform = rot13_transform
};

/* Leetspeak Plugin - Converts letters to numbers
 * a/A -> 4, e/E -> 3, i/I -> 1, o/O -> 0, s/S -> 5
 */
static int leetspeak_transform(const char* input, char* output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return -1;
    }
    
    size_t i;
    size_t len = 0;
    
    for (i = 0; input[i] != '\0' && i < output_size - 1; i++) {
        char c = input[i];
        
        switch (c) {
            case 'a': case 'A': output[i] = '4'; break;
            case 'e': case 'E': output[i] = '3'; break;
            case 'i': case 'I': output[i] = '1'; break;
            case 'o': case 'O': output[i] = '0'; break;
            case 's': case 'S': output[i] = '5'; break;
            default: output[i] = c; break;
        }
        len++;
    }
    
    output[len] = '\0';
    return 0;
}

plugin_t leetspeak_plugin = {
    .name = "leetspeak",
    .transform = leetspeak_transform
};

/* Demo program showing custom plugin usage */
int main(void) {
    const char* message = "Hello world!";
    char output[PLUGIN_MAX_MESSAGE_LEN];
    
    printf("Custom Plugin Example\n");
    printf("=====================\n\n");
    
    /* Test 1: Original message */
    printf("Original message:\n  %s\n\n", message);
    
    /* Test 2: ROT13 plugin */
    register_plugin(&rot13_plugin);
    apply_plugins(message, output, sizeof(output));
    printf("With ROT13 plugin:\n  %s\n\n", output);
    
    /* Test 3: Leetspeak plugin */
    clear_plugins();
    register_plugin(&leetspeak_plugin);
    apply_plugins(message, output, sizeof(output));
    printf("With Leetspeak plugin:\n  %s\n\n", output);
    
    /* Test 4: Both plugins chained */
    clear_plugins();
    register_plugin(&rot13_plugin);
    register_plugin(&leetspeak_plugin);
    apply_plugins(message, output, sizeof(output));
    printf("With ROT13 + Leetspeak (chained):\n  %s\n\n", output);
    
    printf("=====================\n");
    printf("This demonstrates how external developers can\n");
    printf("create and use custom plugins without modifying\n");
    printf("hello.c or any core files!\n");
    
    return 0;
}
