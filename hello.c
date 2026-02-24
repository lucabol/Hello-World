#include <stdio.h>

void print_message(const char *msg) {
    puts(msg);
}

int main(void) {
    int exit_code = 0;
    print_message("Hello world!");
    printf("Exit code: %d\n", exit_code);
    return exit_code;
}