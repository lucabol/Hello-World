/*
 * Unit test program for hello.c
 * 
 * This program uses fork/exec to run the hello program in a separate process
 * and capture its output and exit code for validation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* Global test counters */
int test_count = 0;
int test_passed = 0;

/*
 * Assert that two strings are equal
 * Prints pass/fail status and increments test counters
 */
void assert_equal_string(const char* expected, const char* actual, const char* test_name) {
    test_count++;
    if (strcmp(expected, actual) == 0) {
        printf("✓ PASS: %s\n", test_name);
        test_passed++;
    } else {
        printf("✗ FAIL: %s\n", test_name);
        printf("  Expected: \"%s\"\n", expected);
        printf("  Actual:   \"%s\"\n", actual);
    }
}

/*
 * Assert that two integers are equal
 * Prints pass/fail status and increments test counters
 */
void assert_equal_int(int expected, int actual, const char* test_name) {
    test_count++;
    if (expected == actual) {
        printf("✓ PASS: %s\n", test_name);
        test_passed++;
    } else {
        printf("✗ FAIL: %s\n", test_name);
        printf("  Expected: %d\n", expected);
        printf("  Actual:   %d\n", actual);
    }
}

/*
 * Run the hello program in a separate process and capture its output and exit code
 * 
 * Uses fork() to create a child process that runs the hello program via execl().
 * Uses pipe() to capture the program's stdout output.
 * The parent process reads the output and waits for the child to complete.
 * 
 * Returns: The exit code of the hello program, or -1 on error
 * 
 * Parameters:
 *   output - Buffer to store the captured stdout
 *   output_size - Size of the output buffer
 */
int run_hello_program(char* output, size_t output_size) {
    int pipefd[2];
    pid_t pid;
    int status;
    
    /* Create a pipe for communication between parent and child */
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }
    
    /* Fork to create child process */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    
    if (pid == 0) {
        /* Child process: run the hello program */
        close(pipefd[0]); /* Close read end of pipe */
        dup2(pipefd[1], STDOUT_FILENO); /* Redirect stdout to pipe */
        close(pipefd[1]);
        
        /* Execute the hello program */
        execl("./hello", "./hello", (char*)NULL);
        perror("execl"); /* This should not be reached */
        exit(1);
    } else {
        /* Parent process: capture output and wait for completion */
        close(pipefd[1]); /* Close write end of pipe */
        
        /* Read output from the child process */
        ssize_t bytes_read = read(pipefd[0], output, output_size - 1);
        if (bytes_read >= 0) {
            output[bytes_read] = '\0'; /* Null-terminate the string */
        } else {
            output[0] = '\0'; /* Empty string on error */
        }
        
        close(pipefd[0]);
        
        /* Wait for child process to complete and get its exit status */
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status); /* Return the exit code */
        } else {
            return -1; /* Child terminated abnormally */
        }
    }
}

/*
 * Test function to verify the hello program's output and exit code
 * 
 * This function runs the hello program and validates that it:
 * 1. Outputs the expected message "Hello world!"
 * 2. Exits with code 0 (success)
 */
void test_hello_output() {
    char output[1024];
    int exit_code = run_hello_program(output, sizeof(output));
    
    /* Test the output message */
    assert_equal_string("Hello world!", output, "Hello program outputs correct message");
    
    /* Test the exit code */
    assert_equal_int(0, exit_code, "Hello program exits with code 0");
}

int main() {
    printf("Running unit tests for hello.c\n");
    printf("================================\n");
    
    test_hello_output();
    
    printf("================================\n");
    printf("Tests completed: %d/%d passed\n", test_passed, test_count);
    
    return (test_passed == test_count) ? 0 : 1;
}