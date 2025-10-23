/* Test different function definition styles */
#include <stdio.h>

// Function with brace on same line
int foo() {
    return 1;
}

// K&R style - brace on next line
int bar()
{
    return 2;
}

// Allman style with parameters
int baz(int x, int y)
{
    return x + y;
}

// Function prototype (should not be counted)
int qux(void);

int main()
{
    return 0;
}
