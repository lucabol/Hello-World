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

int main() {
    print_message("Hello world!");
    printf("\n");
    return 0;
}
