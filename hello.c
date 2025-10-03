#include <stdio.h>
#include "hello.h"
#include "plugin.h"

/* Returns the greeting string */
const char* get_greeting(void) {
    const char* message = "Hello world!";
    const char* final_message = apply_plugins(message);
    return final_message;
}

/* Main function - excluded when building unit tests */
#ifndef UNIT_TEST
int main(void) {
    printf("%s", get_greeting());
    return 0;
}
#endif
