# include <stdio.h>
#include "hello.h"

const char* get_greeting(void) {
    return "Ciao, Mondo!";
}

void print_custom_message(const char* message) {
    if (message == NULL) {
        printf("(null message)\n");
        return;
    }
    printf("%s\n", message);
}

#ifndef UNIT_TEST
int main(){
    printf("%s\n", get_greeting());
    return 0;
}
#endif

