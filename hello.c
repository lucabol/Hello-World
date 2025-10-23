#include <stdio.h>
#include "hello.h"

/* Get the greeting message
 * Returns: A pointer to a static string constant
 */
const char* get_greeting(void) {
    return "Ciao, Mondo!";
}

/* UNIT_TEST Compilation Mode:
 * When compiled with -DUNIT_TEST, the main() function is excluded from compilation.
 * This allows hello.c to be compiled as a library (hello_lib.o) that exports only
 * the get_greeting() function, enabling unit tests to link against it without
 * symbol conflicts. The test runner provides its own main() function.
 *
 * Normal mode: Compiles with main() - creates standalone executable
 * Unit test mode: Compiles without main() - creates linkable library for tests
 */
#ifndef UNIT_TEST
int main(void) {
    puts(get_greeting());
    puts("Exit code: 0");
    return 0;
}
#endif
