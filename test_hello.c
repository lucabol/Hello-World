#include "test_framework.h"
#include "hello.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* Global test counters */
int tests_run = 0;
int tests_passed = 0;

/* Unit Tests */
void test_get_greeting() {
    const char* greeting = get_greeting();
    ASSERT_STR_EQUAL("Hello world!", greeting);
}

/* Integration test - capture output of print_greeting function */
void test_print_greeting() {
    int pipefd[2];
    char buffer[256];
    
    /* Create pipe for capturing output */
    if (pipe(pipefd) == -1) {
        printf("FAIL: Could not create pipe for test\n");
        tests_run++;
        return;
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        printf("FAIL: Could not fork for test\n");
        tests_run++;
        return;
    }
    
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
        
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        buffer[bytes_read] = '\0';
        close(pipefd[0]);
        
        /* Wait for child */
        int status;
        waitpid(pid, &status, 0);
        
        ASSERT_STR_EQUAL("Hello world!", buffer);
    }
}

/* Integration test - test full program execution */
void test_full_program() {
    int status = system("cd /home/runner/work/Hello-World/Hello-World && ./hello > /tmp/hello_output.txt 2>&1");
    
    tests_run++;
    if (WEXITSTATUS(status) == 0) {
        tests_passed++;
        printf("PASS: Program exited with code 0\n");
    } else {
        printf("FAIL: Program exited with code %d\n", WEXITSTATUS(status));
    }
    
    /* Check output */
    FILE* file = fopen("/tmp/hello_output.txt", "r");
    if (file) {
        char output[256];
        fgets(output, sizeof(output), file);
        fclose(file);
        
        /* Remove newline if present */
        size_t len = strlen(output);
        if (len > 0 && output[len-1] == '\n') {
            output[len-1] = '\0';
        }
        
        ASSERT_STR_EQUAL("Hello world!", output);
        
        /* Clean up */
        unlink("/tmp/hello_output.txt");
    } else {
        tests_run++;
        printf("FAIL: Could not read program output\n");
    }
}

int main() {
    printf("=== HELLO WORLD UNIT TESTS ===\n\n");
    
    RUN_TEST(test_get_greeting);
    RUN_TEST(test_print_greeting);
    RUN_TEST(test_full_program);
    
    TEST_SUMMARY();
    
    /* Return non-zero if any tests failed */
    return (tests_passed == tests_run) ? 0 : 1;
}