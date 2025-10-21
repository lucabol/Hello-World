/* Main file that uses both translation units */
#include <stdio.h>
#include "../plugin.h"
#include "../hello.h"

extern void function_in_file1(void);
extern void function_in_file2(void);

int main(void) {
    extern int plugin_count;
    
    printf("Testing duplicate plugin includes across TUs\n");
    printf("Plugin count: %d\n", plugin_count);
    
    /* Should be 1, not 2 (deduplication works) */
    if (plugin_count == 1) {
        printf("✓ Deduplication successful - plugin registered only once\n");
        
        /* Test that the plugin actually works */
        const char* result = apply_plugins(get_greeting());
        printf("Result: %s\n", result);
        return 0;
    } else {
        printf("✗ Deduplication failed - plugin registered %d times\n", plugin_count);
        return 1;
    }
}
