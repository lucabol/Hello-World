/* minunit.h - Minimal unit testing framework for C
 * Adapted from: http://www.jera.com/techinfo/jtns/jtn002.html
 * 
 * Usage:
 *   - Use mu_assert(message, test) to assert conditions
 *   - Use mu_run_test(test) to run test functions
 *   - Return test results from test functions
 */

#ifndef MINUNIT_H
#define MINUNIT_H

#include <stdio.h>
#include <string.h>

#define mu_assert(message, test) do { \
    if (!(test)) return message; \
} while (0)

#define mu_run_test(test) do { \
    char *message = test(); \
    tests_run++; \
    if (message) { \
        printf("✗ FAIL: %s\n", message); \
        return message; \
    } else { \
        printf("✓ PASS: %s\n", #test); \
    } \
} while (0)

extern int tests_run;

#endif /* MINUNIT_H */
