#include <stdio.h>
#include "hello.h"

/* Returns the greeting message */
const char* get_greeting(void) {
    return "Ciao, Mondo!";
}

/* Prints a custom message to stdout */
void print_custom_message(const char* message) {
    if (message == NULL) {
        puts("(null)");
    } else {
        puts(message);
    }
}

#ifndef UNIT_TEST
int main(void) {
    int exit_code = 0;
    const char* greeting = get_greeting();
    puts(greeting);
    printf("Exit code: %d\n", exit_code);
    return exit_code;
}
#endif
