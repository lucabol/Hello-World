/* Plugin System Demo
 * Demonstrates various plugin combinations and usage patterns
 */

#include <stdio.h>
#include <stdlib.h>
#include "plugin.h"

/* Declare plugin functions */
extern char* uppercase_plugin(const char* input);
extern char* reverse_plugin(const char* input);
extern char* exclaim_plugin(const char* input);

void demo_single_plugin(void) {
    printf("\n=== Single Plugin Demo ===\n");
    const char* message = "Hello world!";
    
    printf("Original: %s\n", message);
    
    /* Uppercase */
    clear_plugins();
    register_plugin(uppercase_plugin);
    char* result1 = apply_plugins(message);
    printf("Uppercase: %s\n", result1);
    if (result1 != message) free(result1);
    
    /* Reverse */
    clear_plugins();
    register_plugin(reverse_plugin);
    char* result2 = apply_plugins(message);
    printf("Reverse: %s\n", result2);
    if (result2 != message) free(result2);
    
    /* Exclaim */
    clear_plugins();
    register_plugin(exclaim_plugin);
    char* result3 = apply_plugins(message);
    printf("Exclaim: %s\n", result3);
    if (result3 != message) free(result3);
}

void demo_chained_plugins(void) {
    printf("\n=== Chained Plugins Demo ===\n");
    const char* message = "Hello world!";
    
    printf("Original: %s\n", message);
    
    /* Uppercase + Exclaim */
    clear_plugins();
    register_plugin(uppercase_plugin);
    register_plugin(exclaim_plugin);
    char* result1 = apply_plugins(message);
    printf("Uppercase + Exclaim: %s\n", result1);
    if (result1 != message) free(result1);
    
    /* Exclaim + Uppercase */
    clear_plugins();
    register_plugin(exclaim_plugin);
    register_plugin(uppercase_plugin);
    char* result2 = apply_plugins(message);
    printf("Exclaim + Uppercase: %s\n", result2);
    if (result2 != message) free(result2);
    
    /* Uppercase + Reverse */
    clear_plugins();
    register_plugin(uppercase_plugin);
    register_plugin(reverse_plugin);
    char* result3 = apply_plugins(message);
    printf("Uppercase + Reverse: %s\n", result3);
    if (result3 != message) free(result3);
    
    /* All three: Uppercase + Exclaim + Reverse */
    clear_plugins();
    register_plugin(uppercase_plugin);
    register_plugin(exclaim_plugin);
    register_plugin(reverse_plugin);
    char* result4 = apply_plugins(message);
    printf("Uppercase + Exclaim + Reverse: %s\n", result4);
    if (result4 != message) free(result4);
}

void demo_plugin_info(void) {
    printf("\n=== Plugin System Info ===\n");
    
    clear_plugins();
    printf("Initial plugin count: %d\n", get_plugin_count());
    
    register_plugin(uppercase_plugin);
    printf("After registering 1 plugin: %d\n", get_plugin_count());
    
    register_plugin(reverse_plugin);
    register_plugin(exclaim_plugin);
    printf("After registering 3 plugins: %d\n", get_plugin_count());
    
    clear_plugins();
    printf("After clearing: %d\n", get_plugin_count());
}

int main(void) {
    printf("===================================\n");
    printf("Hello World Plugin System Demo\n");
    printf("===================================\n");
    
    demo_single_plugin();
    demo_chained_plugins();
    demo_plugin_info();
    
    printf("\n===================================\n");
    printf("Demo Complete!\n");
    printf("===================================\n");
    
    return 0;
}
