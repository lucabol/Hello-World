#include <stdio.h>
#include "hello.h"

/* Get the default greeting message */
const char* get_greeting(void) {
    return "Hello world!";
}

/* Print a custom message without trailing newline */
void print_custom_message(const char* message) {
    if (message != NULL) {
        printf("%s", message);
    }
}

#ifndef UNIT_TEST
int main(void) {
    print_custom_message(get_greeting());
    return 0;
}
#endif
