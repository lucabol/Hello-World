#include <stdio.h>
#include "hello.h"

/* Print a custom message to stdout */
void print_message(const char* message) {
    printf("%s", message);
}

/* Get the default greeting message */
const char* get_greeting(void) {
    return "Hello world!";
}

#ifndef UNIT_TEST
int main(void) {
    print_message(get_greeting());
    return 0;
}
#endif
