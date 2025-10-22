#include <stdio.h>
#include "hello.h"

/* get_greeting() - Returns the greeting message
 * This function is exposed for unit testing when compiled with -DUNIT_TEST
 */
const char* get_greeting(void) {
    return "Hello world!";
}

#ifndef UNIT_TEST
/* main() - Program entry point (only compiled when not in unit test mode)
 * Prints the greeting message and exit code
 */
int main(void) {
    int exit_code = 0;
    printf("%s\n", get_greeting());
    printf("Exit code: %d\n", exit_code);
    return exit_code;
}
#endif
