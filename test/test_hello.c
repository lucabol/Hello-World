/* test_hello.c - Unit tests for hello.c program
 * Tests output format and exit code using minunit framework
 * 
 * Usage: gcc -o test_hello test_hello.c && ./test_hello
 */

#define _POSIX_C_SOURCE 200809L

#include "minunit.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int tests_run = 0;

/* Test: Program compiles successfully */
static char *test_program_compiles(void) {
    /* Compile the hello.c program with strict flags */
    int result = system("gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -o hello_test hello.c 2>&1");
    mu_assert("Program should compile without errors", WIFEXITED(result) && WEXITSTATUS(result) == 0);
    
    /* Check if binary was created */
    mu_assert("Binary should be created after compilation", access("hello_test", X_OK) == 0);
    
    return 0;
}

/* Test: Program produces correct output */
static char *test_program_output(void) {
    FILE *fp;
    char output[256] = {0};
    size_t bytes_read;
    
    /* Run the program and capture output */
    fp = popen("./hello_test", "r");
    mu_assert("Program should execute successfully", fp != NULL);
    
    bytes_read = fread(output, 1, sizeof(output) - 1, fp);
    output[bytes_read] = '\0';
    
    int status = pclose(fp);
    mu_assert("Program should complete execution", status != -1);
    
    /* Check exact output - should be "Hello world!" without trailing newline */
    mu_assert("Output should be exactly 'Hello world!'", strcmp(output, "Hello world!") == 0);
    mu_assert("Output length should be 12 characters", strlen(output) == 12);
    
    return 0;
}

/* Test: Program exits with code 0 */
static char *test_program_exit_code(void) {
    int status = system("./hello_test > /dev/null 2>&1");
    
    mu_assert("Program should exit normally", WIFEXITED(status));
    mu_assert("Program should exit with code 0", WEXITSTATUS(status) == 0);
    
    return 0;
}

/* Test: Output has no trailing newline */
static char *test_no_trailing_newline(void) {
    FILE *fp;
    char output[256] = {0};
    size_t bytes_read;
    
    /* Run the program and capture output */
    fp = popen("./hello_test", "r");
    mu_assert("Program should execute successfully", fp != NULL);
    
    bytes_read = fread(output, 1, sizeof(output) - 1, fp);
    pclose(fp);
    
    /* Check that output doesn't end with newline */
    mu_assert("Output should not end with newline", bytes_read > 0 && output[bytes_read - 1] != '\n');
    
    return 0;
}

/* Test: Strict compilation with all warnings enabled */
static char *test_strict_compilation(void) {
    /* Test with very strict compiler flags */
    int result = system("gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o hello_strict_test hello.c 2>&1");
    mu_assert("Program should compile with strict flags without warnings", WIFEXITED(result) && WEXITSTATUS(result) == 0);
    
    /* Clean up strict test binary */
    system("rm -f hello_strict_test");
    
    return 0;
}

/* Run all tests */
static char *all_tests(void) {
    mu_run_test(test_program_compiles);
    mu_run_test(test_strict_compilation);
    mu_run_test(test_program_output);
    mu_run_test(test_no_trailing_newline);
    mu_run_test(test_program_exit_code);
    return 0;
}

int main(void) {
    printf("Running Hello World Unit Tests\n");
    printf("================================\n\n");
    
    char *result = all_tests();
    
    printf("\n================================\n");
    if (result != 0) {
        printf("FAILED: %s\n", result);
        printf("Tests run: %d\n", tests_run);
    } else {
        printf("ALL TESTS PASSED\n");
        printf("Tests run: %d\n", tests_run);
    }
    
    /* Clean up test binary */
    system("rm -f hello_test");
    
    return result != 0;
}
