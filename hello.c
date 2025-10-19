#include <stdio.h>
#include "hello.h"

/* Returns the default greeting message */
const char* get_greeting(void) {
    return "Hello world!";
}

/* Prints a custom message passed as an argument */
void print_message(const char* message) {
    printf("%s", message);
}

#ifndef UNIT_TEST
int main() {
    const char* greeting = get_greeting();
    print_message(greeting);
    printf("\n");
    return 0;
}
#endif
