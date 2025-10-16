#include <stdio.h>
#include "hello.h"

const char *get_greeting(void) {
    return "Hello world!";
}

#ifndef UNIT_TEST
int main(void) {
    fputs(get_greeting(), stdout);
    return 0;
}
#endif
