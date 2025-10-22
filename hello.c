# include <stdio.h>
#include "hello.h"

/* Returns the greeting message */
const char* get_greeting(void) {
    static const char* greeting = "Ciao, Mondo!";
    return greeting;
}

#ifndef UNIT_TEST
int main(){
    int exit_code = 0;
    printf("%s\n", get_greeting());
    printf("Exit code: %d\n", exit_code);
    return exit_code;
}
#endif
