# include <stdio.h>
#include "hello.h"

const char* get_greeting(void) {
    return "Hello world!";
}

void print_message(const char* message) {
    printf("%s", message);
}

#ifndef UNIT_TEST
int main(){
    print_message(get_greeting());
}
#endif
