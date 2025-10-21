#include <stdio.h>
#include "hello.h"

/* get_greeting - Returns the greeting message
 * This function returns a static string constant
 */
const char* get_greeting(void) {
    return "Hello world!";
}

/* main - Entry point for the program
 * Prints the greeting message with a newline and exits
 */
#ifndef UNIT_TEST
int main(void) {
    printf("%s\n", get_greeting());
    return 0;
}
#endif
