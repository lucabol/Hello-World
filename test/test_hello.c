#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* Simple unit test framework for hello.c validation */

static int tests_run = 0;
static int tests_passed = 0;

/* Test result reporting */
void test_assert(int condition, const char* test_name) {
    tests_run++;
    if (condition) {
        tests_passed++;
        printf("✓ %s\n", test_name);
    } else {
        printf("✗ %s\n", test_name);
    }
}

/* Capture program output and exit code */
typedef struct {
    char output[256];
    int exit_code;
    int output_length;
} program_result_t;

program_result_t run_hello_program(void) {
    program_result_t result = {0};
    int pipefd[2];
    pid_t pid;
    
    if (pipe(pipefd) == -1) {
        result.exit_code = -1;
        return result;
    }
    
    pid = fork();
    if (pid == -1) {
        result.exit_code = -1;
        return result;
    }
    
    if (pid == 0) {
        /* Child process */
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);
        
        execl("./hello", "./hello", (char*)NULL);
        exit(127); /* execl failed */
    } else {
        /* Parent process */
        int status;
        close(pipefd[1]);
        
        /* Read output */
        result.output_length = read(pipefd[0], result.output, sizeof(result.output) - 1);
        if (result.output_length < 0) {
            result.output_length = 0;
        }
        result.output[result.output_length] = '\0';
        
        close(pipefd[0]);
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            result.exit_code = WEXITSTATUS(status);
        } else {
            result.exit_code = -1;
        }
    }
    
    return result;
}

/* Test: Program outputs correct string */
void test_output_string(void) {
    program_result_t result = run_hello_program();
    const char* expected = "Hello world!";
    
    int correct_output = (result.output_length == 12 && 
                         strncmp(result.output, expected, 12) == 0);
    
    test_assert(correct_output, "Output string is 'Hello world!' (12 bytes, no newline)");
    
    if (!correct_output) {
        printf("  Expected: 'Hello world!' (12 bytes)\n");
        printf("  Actual:   '");
        for (int i = 0; i < result.output_length; i++) {
            if (result.output[i] == '\n') printf("\\n");
            else if (result.output[i] == '\t') printf("\\t");
            else if (result.output[i] == '\r') printf("\\r");
            else printf("%c", result.output[i]);
        }
        printf("' (%d bytes)\n", result.output_length);
    }
}

/* Test: Program exits with code 0 */
void test_exit_code(void) {
    program_result_t result = run_hello_program();
    
    test_assert(result.exit_code == 0, "Program exits with code 0");
    
    if (result.exit_code != 0) {
        printf("  Expected exit code: 0\n");
        printf("  Actual exit code:   %d\n", result.exit_code);
    }
}

/* Test: Program binary exists and is executable */
void test_binary_exists(void) {
    int accessible = (access("./hello", X_OK) == 0);
    test_assert(accessible, "Hello binary exists and is executable");
}

/* Main test runner */
int main(void) {
    printf("Running C unit tests for hello.c\n");
    printf("================================\n");
    
    /* Run all tests */
    test_binary_exists();
    test_output_string();
    test_exit_code();
    
    printf("================================\n");
    printf("Tests run: %d, Passed: %d, Failed: %d\n", 
           tests_run, tests_passed, tests_run - tests_passed);
    
    if (tests_passed == tests_run) {
        printf("All tests PASSED!\n");
        return 0;
    } else {
        printf("Some tests FAILED!\n");
        return 1;
    }
}