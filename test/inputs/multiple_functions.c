/* Test file with multiple functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

void print_result(int result) {
    printf("Result: %d\n", result);
}

char* get_message() {
    return "Hello";
}

int main() {
    int x = add(5, 3);
    int y = subtract(10, 4);
    print_result(x);
    print_result(y);
    return 0;
}
