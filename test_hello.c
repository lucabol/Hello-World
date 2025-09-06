#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "hello.h"

/* Test function prototypes */
static void test_print_greeting_return_value(void);
static void test_print_greeting_output(void);
static void test_hello_program_exit_code(void);
static void test_hello_program_output(void);

/* Initialize test suite */
static int init_suite(void) {
    return 0;
}

/* Clean up test suite */
static int clean_suite(void) {
    return 0;
}

/**
 * Test that print_greeting returns 0 (success)
 */
static void test_print_greeting_return_value(void) {
    /* Redirect stdout to prevent output during test */
    FILE *old_stdout = stdout;
    stdout = fopen("/dev/null", "w");
    
    int result = print_greeting();
    
    /* Restore stdout */
    fclose(stdout);
    stdout = old_stdout;
    
    CU_ASSERT_EQUAL(result, 0);
}

/**
 * Test that print_greeting produces correct output
 */
static void test_print_greeting_output(void) {
    /* Create a pipe to capture output */
    int pipefd[2];
    CU_ASSERT_EQUAL(pipe(pipefd), 0);
    
    /* Fork process to capture output */
    pid_t pid = fork();
    CU_ASSERT(pid >= 0);
    
    if (pid == 0) {
        /* Child process */
        close(pipefd[0]); /* Close read end */
        dup2(pipefd[1], STDOUT_FILENO); /* Redirect stdout to pipe */
        close(pipefd[1]);
        
        print_greeting();
        exit(0);
    } else {
        /* Parent process */
        close(pipefd[1]); /* Close write end */
        
        char buffer[256];
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        CU_ASSERT(bytes_read > 0);
        
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            CU_ASSERT_STRING_EQUAL(buffer, "Hello world!");
        }
        
        close(pipefd[0]);
        
        /* Wait for child process */
        int status;
        waitpid(pid, &status, 0);
        CU_ASSERT_EQUAL(WEXITSTATUS(status), 0);
    }
}

/**
 * Test that the hello program exits with code 0
 */
static void test_hello_program_exit_code(void) {
    int status = system("cd /home/runner/work/Hello-World/Hello-World && ./hello > /dev/null 2>&1");
    CU_ASSERT_EQUAL(WEXITSTATUS(status), 0);
}

/**
 * Test that the hello program produces correct output
 */
static void test_hello_program_output(void) {
    FILE *fp = popen("cd /home/runner/work/Hello-World/Hello-World && ./hello", "r");
    CU_ASSERT_PTR_NOT_NULL(fp);
    
    if (fp != NULL) {
        char buffer[256];
        char *result = fgets(buffer, sizeof(buffer), fp);
        CU_ASSERT_PTR_NOT_NULL(result);
        
        if (result != NULL) {
            /* Remove newline if present */
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len-1] == '\n') {
                buffer[len-1] = '\0';
            }
            CU_ASSERT_STRING_EQUAL(buffer, "Hello world!");
        }
        
        int status = pclose(fp);
        CU_ASSERT_EQUAL(WEXITSTATUS(status), 0);
    }
}

/**
 * Main test runner
 */
int main(void) {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    /* Add a suite to the registry */
    pSuite = CU_add_suite("Hello World Test Suite", init_suite, clean_suite);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add tests to the suite */
    if ((CU_add_test(pSuite, "test print_greeting return value", test_print_greeting_return_value) == NULL) ||
        (CU_add_test(pSuite, "test print_greeting output", test_print_greeting_output) == NULL) ||
        (CU_add_test(pSuite, "test hello program exit code", test_hello_program_exit_code) == NULL) ||
        (CU_add_test(pSuite, "test hello program output", test_hello_program_output) == NULL)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    /* Get test results */
    int num_failures = CU_get_number_of_failures();
    
    /* Clean up registry and return */
    CU_cleanup_registry();
    return (num_failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}