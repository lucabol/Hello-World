#include <stdio.h>
#include "hello.h"

/* Returns the greeting string - Italian version: "Ciao, Mondo!" */
const char* get_greeting(void) {
    return "Ciao, Mondo!";
}

/* Main function - excluded when building unit tests */
#ifndef UNIT_TEST
int main(void) {
    printf("%s", get_greeting());
    return 0;
}
#endif
