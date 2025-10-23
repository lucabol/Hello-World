#include <stdio.h>
#include "hello.h"

/* Get the greeting message */
const char* get_greeting(void) {
    static const char* greeting = "Ciao, Mondo!";
    return greeting;
}

/* Print a custom message (NULL-safe) */
void print_custom_message(const char* message) {
    if (message == NULL) {
        printf("(null message)\n");
    } else {
        printf("%s\n", message);
    }
}

#ifndef UNIT_TEST
int main(void) {
    int exit_code = 0;
    puts(get_greeting());
    printf("Exit code: %d\n", exit_code);
    return exit_code;
}
#endif
