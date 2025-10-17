/* main.c - Hello World Program Runner
 * 
 * Main entry point that uses the hello library to print the greeting.
 * This file contains only I/O logic, the greeting itself comes from the library.
 */

#include <stdio.h>
#include "hello.h"

int main(void) {
    printf("%s", get_greeting());
    return 0;
}
