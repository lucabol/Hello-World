#include <stdio.h>
#include "hello.h"

/* get_greeting - Returns the greeting message
 * This function provides the core greeting functionality
 */
const char* get_greeting(void) {
    return "Hello world!";
}

#ifndef UNIT_TEST
/* main - Entry point of the program
 * Prints the greeting message followed by a newline
 */
int main(void) {
    printf("%s\n", get_greeting());
    return 0;
}
#endif
