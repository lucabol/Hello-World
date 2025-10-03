#include <stdio.h>
#include "hello.h"

/* Get the greeting message */
const char* get_greeting(void) {
    return "Hello world!";
}

#ifndef UNIT_TEST
int main(){
    printf("%s", get_greeting());
    return 0;
}
#endif

