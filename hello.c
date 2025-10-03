# include <stdio.h>
#include "hello.h"

/* Get the greeting message - returns a static string constant */
const char* get_greeting(void) {
    static const char* greeting = "Hello world!";
    return greeting;
}

#ifndef UNIT_TEST
int main(){
    printf("%s", get_greeting());
    return 0;
}
#endif
