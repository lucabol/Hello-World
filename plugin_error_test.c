/* Error test plugin - deliberately returns error to test error handling */
#include "plugin.h"
#include <stdio.h>

/* Transform that always fails to test error propagation */
int error_transform(const char* input, char* output, size_t output_size) {
    (void)input;    /* Unused */
    (void)output;   /* Unused */
    (void)output_size;  /* Unused */
    
    /* Deliberately return an error */
    fprintf(stderr, "[ERROR_TEST_PLUGIN] Returning error as expected\n");
    return PLUGIN_ERROR_INTERNAL;
}

void error_before(void) {
    printf("[ERROR_TEST_PLUGIN] Before hook executed\n");
}

void error_after(void) {
    printf("[ERROR_TEST_PLUGIN] After hook executed (even after error)\n");
}

PLUGIN_REGISTER(ErrorTest, error_transform, error_before, error_after);
