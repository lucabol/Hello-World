#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* Simple test framework for hello.c */

int test_count = 0;
int test_passed = 0;

void assert_equal_string(const char* expected, const char* actual, const char* test_name) {
    test_count++;
    if (strcmp(expected, actual) == 0) {
        printf("PASS: %s\n", test_name);
        test_passed++;
    } else {
        printf("FAIL: %s\n", test_name);
        printf("  Expected: '%s'\n", expected);
        printf("  Actual:   '%s'\n", actual);
    }
}

void assert_equal_int(int expected, int actual, const char* test_name) {
    test_count++;
    if (expected == actual) {
        printf("PASS: %s\n", test_name);
        test_passed++;
    } else {
        printf("FAIL: %s\n", test_name);
        printf("  Expected: %d\n", expected);
        printf("  Actual:   %d\n", actual);
    }
}

/* Capture output and exit code from running hello executable */
int run_hello_and_capture(char* output_buffer, size_t buffer_size) {
    int pipefd[2];
    pid_t pid;
    int status;
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }
    
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    
    if (pid == 0) {
        /* Child process */
        close(pipefd[0]); /* Close read end */
        dup2(pipefd[1], STDOUT_FILENO); /* Redirect stdout to pipe */
        close(pipefd[1]);
        
        execl("./hello", "hello", (char*)NULL);
        perror("execl");
        exit(1);
    } else {
        /* Parent process */
        close(pipefd[1]); /* Close write end */
        
        ssize_t bytes_read = read(pipefd[0], output_buffer, buffer_size - 1);
        if (bytes_read >= 0) {
            output_buffer[bytes_read] = '\0';
        } else {
            output_buffer[0] = '\0';
        }
        
        close(pipefd[0]);
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return -1;
        }
    }
}

void test_hello_output() {
    char output[256];
    int exit_code = run_hello_and_capture(output, sizeof(output));
    
    assert_equal_string("Hello world!", output, "Hello program outputs correct message");
    assert_equal_int(0, exit_code, "Hello program exits with code 0");
}

int main() {
    printf("Running tests for hello.c...\n\n");
    
    /* Check if hello executable exists */
    if (access("./hello", X_OK) != 0) {
        printf("ERROR: ./hello executable not found or not executable.\n");
        printf("Please compile with: gcc -o hello hello.c\n");
        return 1;
    }
    
    test_hello_output();
    
    printf("\nTest Results:\n");
    printf("Tests run: %d\n", test_count);
    printf("Tests passed: %d\n", test_passed);
    printf("Tests failed: %d\n", test_count - test_passed);
    
    if (test_passed == test_count) {
        printf("\nAll tests PASSED!\n");
        return 0;
    } else {
        printf("\nSome tests FAILED!\n");
        return 1;
    }
}