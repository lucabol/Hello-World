#include <stdio.h>
#include "hello.h"

/* Get the greeting message */
const char* get_greeting(void) {
    return "Ciao, Mondo!";
}

#ifndef UNIT_TEST
int main() {
    puts(get_greeting());
    puts("Exit code: 0");
    return 0;
}
#endif
