/* Separate test to verify SIMPLE_TEST_NO_COLOR=1 disables colors
 * This must be run as a separate process to test environment variable handling
 */
#include <stdio.h>
#include <stdlib.h>
#include "simple_test.h"

int main(void) {
    simple_test_init();
    
    int colors = use_colors();
    
    /* When SIMPLE_TEST_NO_COLOR=1, colors should be disabled */
    if (colors == 0) {
        printf("PASS: Colors are disabled when SIMPLE_TEST_NO_COLOR=1\n");
        return 0;
    } else {
        printf("FAIL: Colors are still enabled when SIMPLE_TEST_NO_COLOR=1\n");
        return 1;
    }
}
