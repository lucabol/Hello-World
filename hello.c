#include <stdio.h>
#include "hello.h"

/* Get the greeting message
 * Returns a static string constant
 */
const char* get_greeting(void) {
    return "Hello world!";
}

#ifndef UNIT_TEST
int main(void) {
    printf("%s\n", get_greeting());
    return 0;
}
#endif
