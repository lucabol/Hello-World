#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int test_count = 0;
int test_passed = 0;

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

int run_hello_program(char* output, size_t output_size) {
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
        // Child process
        close(pipefd[0]); // Close read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[1]);
        
        execl("./hello", "./hello", (char*)NULL);
        perror("execl"); // This should not be reached
        exit(1);
    } else {
        // Parent process
        close(pipefd[1]); // Close write end
        
        ssize_t bytes_read = read(pipefd[0], output, output_size - 1);
        if (bytes_read >= 0) {
            output[bytes_read] = '\0';
        } else {
            output[0] = '\0';
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
    char output[1024];
    int exit_code = run_hello_program(output, sizeof(output));
    
    // Test the output message
    assert_equal_string("Hello world!", output, "Hello program outputs correct message");
    
    // Test the exit code
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