#include <stdio.h>
#include "hello.h"

/* Function to print a custom message
 * Parameters:
 *   message - the string to print (must not be NULL)
 */
void print_message(const char* message) {
    if (message != NULL) {
        printf("%s", message);
    }
}

/* Function to get the greeting message
 * Returns: pointer to a static string constant "Hello world!"
 */
const char* get_greeting(void) {
    return "Hello world!";
}

#ifndef UNIT_TEST
int main() {
    print_message("Hello world!");
    printf("\n");
    return 0;
}
#endif
