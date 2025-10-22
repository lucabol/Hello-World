#include <stdio.h>
#include "hello.h"

/* Get the greeting message */
const char* get_greeting(void) {
    return "Ciao, Mondo!";
}

/* Print a custom message to stdout */
void print_custom_message(const char* message) {
    if (message != NULL) {
        printf("%s", message);
    }
}

#ifndef UNIT_TEST
int main(void) {
    const char* greeting = get_greeting();
    print_custom_message(greeting);
    printf("\nExit code: 0\n");
    return 0;
}
#endif
