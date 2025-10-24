# include <stdio.h>
#include "hello.h"

/* Returns the greeting message */
const char* get_greeting(void) {
    return "Ciao, Mondo!";
}

/* Prints a custom message, or default if NULL */
void print_custom_message(const char* message) {
    if (message == NULL) {
        printf("%s\n", get_greeting());
    } else {
        printf("%s\n", message);
    }
}

#ifndef UNIT_TEST
int main(){
    int exit_code = 0;
    printf("%s\n", get_greeting());
    printf("Exit code: %d\n", exit_code);
    return exit_code;
}
#endif
