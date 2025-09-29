#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// ANSI color codes for output
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YELLOW "\033[1;33m"
#define NC "\033[0m" // No Color

// Expected values
#define EXPECTED_OUTPUT "Hello world!"
#define EXPECTED_EXIT_CODE 0

// Function to run the hello program and capture output and exit code
int run_hello_program(char *output_buffer, size_t buffer_size, int *exit_code) {
    int pipe_fd[2];
    pid_t pid;
    
    if (pipe(pipe_fd) == -1) {
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
        close(pipe_fd[0]); // Close read end
        dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipe_fd[1]);
        
        // Execute the hello program
        execl("./hello", "./hello", (char *)NULL);
        perror("execl");
        exit(127);
    } else {
        // Parent process
        close(pipe_fd[1]); // Close write end
        
        // Read output from child
        ssize_t bytes_read = read(pipe_fd[0], output_buffer, buffer_size - 1);
        if (bytes_read >= 0) {
            output_buffer[bytes_read] = '\0';
        }
        close(pipe_fd[0]);
        
        // Wait for child and get exit code
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            *exit_code = WEXITSTATUS(status);
        } else {
            *exit_code = -1;
        }
        
        return bytes_read;
    }
}

int main() {
    printf(YELLOW "Running Hello World Unit Tests...\n" NC);
    
    char output[256];
    int exit_code;
    int tests_passed = 0;
    int total_tests = 2;
    
    // Test 1: Check if hello program exists
    if (access("./hello", F_OK) != 0) {
        printf(RED "✗ Test Setup: hello program not found. Please compile first.\n" NC);
        printf("  Run: gcc -o hello hello.c\n");
        return 1;
    }
    
    // Run the hello program
    int result = run_hello_program(output, sizeof(output), &exit_code);
    if (result < 0) {
        printf(RED "✗ Failed to run hello program\n" NC);
        return 1;
    }
    
    // Test 1: Verify output
    printf("\nTest 1: Output Verification\n");
    if (strcmp(output, EXPECTED_OUTPUT) == 0) {
        printf(GREEN "✓ Output matches expected: \"%s\"\n" NC, EXPECTED_OUTPUT);
        tests_passed++;
    } else {
        printf(RED "✗ Output mismatch!\n" NC);
        printf("  Expected: \"%s\"\n", EXPECTED_OUTPUT);
        printf("  Actual:   \"%s\"\n", output);
        printf("  Expected length: %zu\n", strlen(EXPECTED_OUTPUT));
        printf("  Actual length:   %zu\n", strlen(output));
    }
    
    // Test 2: Verify exit code
    printf("\nTest 2: Exit Code Verification\n");
    if (exit_code == EXPECTED_EXIT_CODE) {
        printf(GREEN "✓ Exit code matches expected: %d\n" NC, EXPECTED_EXIT_CODE);
        tests_passed++;
    } else {
        printf(RED "✗ Exit code mismatch!\n" NC);
        printf("  Expected: %d\n", EXPECTED_EXIT_CODE);
        printf("  Actual:   %d\n", exit_code);
    }
    
    // Summary
    printf("\n" YELLOW "Test Summary:\n" NC);
    printf("  Tests passed: %d/%d\n", tests_passed, total_tests);
    
    if (tests_passed == total_tests) {
        printf(GREEN "✓ All tests passed!\n" NC);
        return 0;
    } else {
        printf(RED "✗ Some tests failed!\n" NC);
        return 1;
    }
}