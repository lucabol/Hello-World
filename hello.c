#include <stdio.h>
#include "hello.h"

/* Function to get the greeting message
 * Returns: pointer to a static string constant "Hello world!"
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
