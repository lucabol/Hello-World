#include <stdio.h>

#ifdef UNIT_TEST
#include "hello.h"

/* Get the greeting message for unit testing */
const char* get_greeting(void) {
    return "Hello world!";
}
#else
int main(void) {
    printf("Hello world!");
    return 0;
}
#endif
