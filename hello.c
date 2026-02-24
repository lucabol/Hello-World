#include <stdio.h>

void print_message(const char *msg) {
    puts(msg);
}

int main(void) {
    print_message("Hello world!");
    return 0;
}