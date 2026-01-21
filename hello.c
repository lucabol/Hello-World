#include <stdio.h>
#include <stdlib.h>

/* Main function to print greeting */
#ifndef UNIT_TEST
int main(){
#else
int hello_main(){
#endif
    if (printf("Hello world!") < 0) {
        fprintf(stderr, "Error: Failed to write output\n");
        return EXIT_FAILURE;
    }
    if (fflush(stdout) != 0) {
        fprintf(stderr, "Error: Failed to flush output\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
