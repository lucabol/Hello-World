# include <stdio.h>
#include "hello.h"

/* Get the default greeting message */
const char* get_greeting(void) {
    return "Ciao, Mondo!";
}

/* Print a custom message */
void print_custom_message(const char* message) {
    if (message == NULL) {
        printf("(null message)");
    } else {
        printf("%s", message);
    }
}

#ifndef UNIT_TEST
int main(){
    const char* greeting = get_greeting();
    print_custom_message(greeting);
    printf("\nExit code: 0\n");
    return 0;
}
#endif
