#include <stdio.h>

const char* get_greeting(void) {
    return "Hello world!";
}

#ifndef UNIT_TEST
int main(void) {
    printf("%s", get_greeting());
    return 0;
}
#endif
