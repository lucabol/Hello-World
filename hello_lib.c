#include <stdio.h>
#include "hello.h"

/* Function to get the greeting message */
const char* get_greeting() {
    return "Hello world!";
}

/* Function to print the greeting */
void print_greeting() {
    printf("%s", get_greeting());
}