#include <stdio.h>
#include "hello.h"

/* Returns the greeting string */
const char* get_greeting(void) {
    return "Hello world!";
}

/* Main function - excluded when building unit tests */
#ifndef UNIT_TEST
int main(void) {
    int exit_code = 0;
    printf("%s", get_greeting());
    printf("\nExit code: %d", exit_code);
    return exit_code;
}
#endif
