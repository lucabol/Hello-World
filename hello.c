#include <stdio.h>
#include "hello.h"

/* Get the greeting message */
const char* get_greeting(void) {
    return "Ciao, Mondo!";
}

#ifndef UNIT_TEST
int main(void) {
    printf("%s\n", get_greeting());
    printf("Exit code: 0\n");
    return 0;
}
#endif
