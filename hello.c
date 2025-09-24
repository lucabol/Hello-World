#include <stdio.h>

/**
 * Prints a custom message to standard output with a newline
 * @param message The string to print
 */
void print_message(const char* message){
    printf("%s\n", message);
}

int main(void){
    int rc = 0;
    print_message("Hello world!");
    printf("Exit code: %d\n", rc);
    return rc;
}
