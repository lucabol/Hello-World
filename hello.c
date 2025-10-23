#include <stdio.h>
#include "hello.h"

/* Get the greeting message */
const char* get_greeting(void) {
    return "Ciao, Mondo!";
}

/* Print a custom message with newline */
void print_custom_message(const char* msg) {
    if (msg != NULL) {
        printf("%s\n", msg);
    }
}

#ifndef UNIT_TEST
int main(void) {
    printf("%s\n", get_greeting());
    return 0;
}
#endif
