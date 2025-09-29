#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

// ANSI color codes for output
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YELLOW "\033[1;33m"
#define NC "\033[0m" // No Color

// Expected values
#define EXPECTED_OUTPUT "Hello world!"
#define EXPECTED_EXIT_CODE 0
#define MAX_OUTPUT_SIZE 1024

// Global flag for quiet mode
static int quiet_mode = 0;

// Function to print messages respecting quiet mode
void print_message(const char* color, const char* prefix, const char* message) {
    if (quiet_mode) {
        printf("%s %s\n", prefix, message);
    } else {
        printf("%s%s %s%s\n", color, prefix, message, NC);
    }
}

void print_success(const char* message) {
    print_message(GREEN, "✓", message);
}

void print_error(const char* message) {
    print_message(RED, "✗", message);
}

void print_info(const char* message) {
    print_message(YELLOW, "ℹ", message);
}

// Function to run the hello program and capture output and exit code
int run_hello_program(char *output_buffer, size_t buffer_size, int *exit_code, size_t *total_bytes_read) {
    int pipe_fd[2];
    pid_t pid;
    
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return -1;
    }
    
    pid = fork();
    if (pid == -1) {
        perror("fork");
        if (close(pipe_fd[0]) == -1) perror("close pipe_fd[0]");
        if (close(pipe_fd[1]) == -1) perror("close pipe_fd[1]");
        return -1;
    }
    
    if (pid == 0) {
        // Child process
        if (close(pipe_fd[0]) == -1) { // Close read end
            perror("close pipe_fd[0] in child");
            exit(127);
        }
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) { // Redirect stdout to pipe
            perror("dup2");
            exit(127);
        }
        if (close(pipe_fd[1]) == -1) {
            perror("close pipe_fd[1] in child");
            exit(127);
        }
        
        // Execute the hello program
        execl("./hello", "./hello", (char *)NULL);
        perror("execl");
        exit(127);
    } else {
        // Parent process
        if (close(pipe_fd[1]) == -1) { // Close write end
            perror("close pipe_fd[1] in parent");
        }
        
        // Read output from child using a loop to handle partial reads
        size_t bytes_accumulated = 0;
        ssize_t bytes_read;
        
        while (bytes_accumulated < buffer_size - 1) {
            bytes_read = read(pipe_fd[0], output_buffer + bytes_accumulated, 
                            buffer_size - 1 - bytes_accumulated);
            
            if (bytes_read == -1) {
                if (errno == EINTR) {
                    continue; // Interrupted by signal, retry
                }
                perror("read from pipe");
                break;
            } else if (bytes_read == 0) {
                break; // EOF reached
            } else {
                bytes_accumulated += (size_t)bytes_read;
            }
        }
        
        // NUL-terminate at the correct position
        output_buffer[bytes_accumulated] = '\0';
        *total_bytes_read = bytes_accumulated;
        
        if (close(pipe_fd[0]) == -1) {
            perror("close pipe_fd[0] in parent");
        }
        
        // Wait for child and get exit code
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            *exit_code = -1;
            return -1;
        }
        
        if (WIFEXITED(status)) {
            *exit_code = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            *exit_code = -1;
            if (!quiet_mode) {
                printf("Child process terminated by signal %d\n", WTERMSIG(status));
            }
        } else {
            *exit_code = -1;
            if (!quiet_mode) {
                printf("Child process terminated abnormally\n");
            }
        }
        
        return (int)bytes_accumulated;
    }
}

int main(int argc, char *argv[]) {
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--quiet") == 0) {
            quiet_mode = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [--quiet] [--help]\n", argv[0]);
            printf("  --quiet  Suppress colors and non-essential output\n");
            printf("  --help   Show this help message\n");
            return 0;
        }
    }
    
    if (!quiet_mode) {
        printf(YELLOW "Running Hello World Unit Tests...\n" NC);
    } else {
        printf("Running Hello World Unit Tests...\n");
    }
    
    char output[MAX_OUTPUT_SIZE];
    int exit_code;
    int tests_passed = 0;
    int total_tests = 2;
    size_t bytes_read = 0;
    
    // Test 1: Check if hello program exists
    if (access("./hello", F_OK) != 0) {
        print_error("Test Setup: hello program not found. Please compile first.");
        if (!quiet_mode) {
            printf("  Run: gcc -o hello hello.c\n");
        }
        return 1;
    }
    
    // Run the hello program
    int result = run_hello_program(output, sizeof(output), &exit_code, &bytes_read);
    if (result < 0) {
        print_error("Failed to run hello program");
        return 1;
    }
    
    // Test 1: Verify output with explicit trailing newline check
    if (!quiet_mode) {
        printf("\nTest 1: Output Verification\n");
    }
    
    size_t expected_len = strlen(EXPECTED_OUTPUT);
    int output_correct = 1;
    
    // Check exact length match
    if (bytes_read != expected_len) {
        output_correct = 0;
    }
    
    // Check content match
    if (strcmp(output, EXPECTED_OUTPUT) != 0) {
        output_correct = 0;
    }
    
    // Explicitly verify no trailing newline
    if (bytes_read > 0 && output[bytes_read - 1] == '\n') {
        output_correct = 0;
        if (!quiet_mode) {
            printf("  Error: Output contains unexpected trailing newline\n");
        }
    }
    
    // Verify buffer is properly NUL-terminated at correct position
    if (output[bytes_read] != '\0') {
        output_correct = 0;
        if (!quiet_mode) {
            printf("  Error: Output buffer not properly NUL-terminated\n");
        }
    }
    
    if (output_correct) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Output matches expected: \"%s\"", EXPECTED_OUTPUT);
        print_success(msg);
        tests_passed++;
    } else {
        print_error("Output mismatch!");
        if (!quiet_mode) {
            printf("  Expected: \"%s\"\n", EXPECTED_OUTPUT);
            printf("  Actual:   \"%s\"\n", output);
            printf("  Expected length: %zu\n", expected_len);
            printf("  Actual length:   %zu\n", bytes_read);
            if (bytes_read > 0 && output[bytes_read - 1] == '\n') {
                printf("  Issue: Output has unexpected trailing newline\n");
            }
        }
    }
    
    // Test 2: Verify exit code
    if (!quiet_mode) {
        printf("\nTest 2: Exit Code Verification\n");
    }
    
    if (exit_code == EXPECTED_EXIT_CODE) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Exit code matches expected: %d", EXPECTED_EXIT_CODE);
        print_success(msg);
        tests_passed++;
    } else {
        print_error("Exit code mismatch!");
        if (!quiet_mode) {
            printf("  Expected: %d\n", EXPECTED_EXIT_CODE);
            printf("  Actual:   %d\n", exit_code);
            if (exit_code == -1) {
                printf("  Note: Exit code -1 indicates abnormal termination (signal or other error)\n");
            }
        }
    }
    
    // Summary
    if (!quiet_mode) {
        printf("\n" YELLOW "Test Summary:\n" NC);
        printf("  Tests passed: %d/%d\n", tests_passed, total_tests);
    } else {
        printf("Test Summary: %d/%d tests passed\n", tests_passed, total_tests);
    }
    
    if (tests_passed == total_tests) {
        print_success("All tests passed!");
        return 0;
    } else {
        print_error("Some tests failed!");
        return 1;
    }
}