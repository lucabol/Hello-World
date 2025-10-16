# include <stdio.h>
#include "hello.h"

const char* get_greeting(void) {
    return "Hello world!";
}

#ifndef UNIT_TEST
int main(){
    printf("%s", get_greeting());
}
#endif
