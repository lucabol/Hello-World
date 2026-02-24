#include <stdio.h>

void print_message(const char *msg)
{
    fputs(msg, stdout);
}

int main(void)
{
    print_message("Hello world!");
    return 0;
}
