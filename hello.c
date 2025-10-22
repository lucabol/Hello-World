#include <stdio.h>
#include "hello.h"

/* Get the greeting message */
const char* get_greeting(void) {
    return "Ciao, Mondo!";
}

#ifndef UNIT_TEST
int main(void) {
    int rc = 0;
    printf("%s\n", get_greeting());
    printf("Exit code: %d\n", rc);
    return rc;
}
#endif
