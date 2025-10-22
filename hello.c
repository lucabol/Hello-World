#include <stdio.h>
#include "hello.h"

/* Get the greeting message */
const char* get_greeting(void) {
    return "Ciao, Mondo!";
}

/* Print a custom message to stdout */
void print_custom_message(const char* message) {
    if (message != NULL) {
        fputs(message, stdout);
    }
}

#ifndef UNIT_TEST
int main(void) {
    const char* greeting = get_greeting();
    print_custom_message(greeting);
    putchar('\n');
    return 0;
}
#endif

