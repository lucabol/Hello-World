/* Simple test to verify deduplication works */
#include <stdio.h>
#include "../plugin.h"

static char buf[256];
const char* test_func(const char* msg) {
    return msg;
}

int main(void) {
    extern int plugin_count;
    
    /* Try to register the same function twice */
    int r1 = register_plugin(test_func);
    int r2 = register_plugin(test_func);
    
    printf("First registration: %d\n", r1);
    printf("Second registration: %d\n", r2);
    printf("Plugin count: %d\n", plugin_count);
    
    if (r1 == 0 && r2 == -2 && plugin_count == 1) {
        printf("✓ Deduplication works for manual registration\n");
        return 0;
    } else {
        printf("✗ Deduplication failed\n");
        return 1;
    }
}
