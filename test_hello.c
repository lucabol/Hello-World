#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// Simple test framework
#define RESET_COLOR "\033[0m"
#define GREEN_COLOR "\033[0;32m"
#define RED_COLOR "\033[0;31m"
#define BLUE_COLOR "\033[0;34m"

static int test_count = 0;
static int test_passed = 0;

void assert_equals_str(const char* expected, const char* actual, const char* test_name) {
    test_count++;
    if (strcmp(expected, actual) == 0) {
        printf(GREEN_COLOR "✓ PASS" RESET_COLOR ": %s\n", test_name);
        test_passed++;
    } else {
        printf(RED_COLOR "✗ FAIL" RESET_COLOR ": %s\n", test_name);
        printf("  Expected: '%s'\n", expected);
        printf("  Actual:   '%s'\n", actual);
    }
}

void assert_equals_int(int expected, int actual, const char* test_name) {
    test_count++;
    if (expected == actual) {
        printf(GREEN_COLOR "✓ PASS" RESET_COLOR ": %s\n", test_name);
        test_passed++;
    } else {
        printf(RED_COLOR "✗ FAIL" RESET_COLOR ": %s\n", test_name);
        printf("  Expected: %d\n", expected);
        printf("  Actual:   %d\n", actual);
    }
}

// Test helper function to capture program output and exit code
typedef struct {
    char output[1024];
    int exit_code;
} program_result_t;

program_result_t run_hello_program() {
    program_result_t result = {0};
    int pipefd[2];
    pid_t pid;
    
    // Create pipe for capturing stdout
    if (pipe(pipefd) == -1) {
        perror("pipe");
        result.exit_code = -1;
        return result;
    }
    
    pid = fork();
    if (pid == -1) {
        perror("fork");
        result.exit_code = -1;
        return result;
    }
    
    if (pid == 0) {
        // Child process
        close(pipefd[0]); // Close read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[1]);
        
        // Execute the hello program
        execl("./hello", "./hello", (char*)NULL);
        perror("execl");
        exit(1);
    } else {
        // Parent process
        close(pipefd[1]); // Close write end
        
        // Read output from pipe
        ssize_t bytes_read = read(pipefd[0], result.output, sizeof(result.output) - 1);
        if (bytes_read > 0) {
            result.output[bytes_read] = '\0';
        }
        close(pipefd[0]);
        
        // Wait for child and get exit status
        int status;
        waitpid(pid, &status, 0);
        result.exit_code = WEXITSTATUS(status);
    }
    
    return result;
}

// Test functions
void test_hello_output() {
    printf(BLUE_COLOR "\n=== Testing Hello Program Output ===" RESET_COLOR "\n");
    
    program_result_t result = run_hello_program();
    
    // Test the expected output
    assert_equals_str("Hello world!", result.output, "Output matches expected 'Hello world!'");
}

void test_hello_exit_code() {
    printf(BLUE_COLOR "\n=== Testing Hello Program Exit Code ===" RESET_COLOR "\n");
    
    program_result_t result = run_hello_program();
    
    // Test the expected exit code
    assert_equals_int(0, result.exit_code, "Exit code is 0 (success)");
}

void test_compilation() {
    printf(BLUE_COLOR "\n=== Testing Compilation ===" RESET_COLOR "\n");
    
    // Test compilation with warnings
    int compile_result = system("gcc -Wall -Wextra -o hello hello.c 2>/dev/null");
    assert_equals_int(0, WEXITSTATUS(compile_result), "Compiles without errors with -Wall -Wextra");
    
    // Test strict compilation
    compile_result = system("gcc -Wall -Wextra -Wpedantic -Wformat=2 -o hello hello.c 2>/dev/null");
    assert_equals_int(0, WEXITSTATUS(compile_result), "Compiles without errors with strict warnings");
}

void test_program_exists_after_compilation() {
    printf(BLUE_COLOR "\n=== Testing Program Existence ===" RESET_COLOR "\n");
    
    // Ensure the binary exists and is executable
    int result = access("./hello", X_OK);
    assert_equals_int(0, result, "hello binary exists and is executable");
}

int main() {
    printf(BLUE_COLOR "====================================\n");
    printf("     Hello.c Unit Test Suite\n");
    printf("====================================" RESET_COLOR "\n");
    
    // Compile the program first
    printf("\nCompiling hello.c...\n");
    if (system("gcc -Wall -Wextra -o hello hello.c") != 0) {
        printf(RED_COLOR "✗ FAIL: Could not compile hello.c" RESET_COLOR "\n");
        return 1;
    }
    printf(GREEN_COLOR "✓ Compilation successful" RESET_COLOR "\n");
    
    // Run all tests
    test_compilation();
    test_program_exists_after_compilation();
    test_hello_output();
    test_hello_exit_code();
    
    // Print summary
    printf(BLUE_COLOR "\n====================================\n");
    printf("          Test Summary\n");
    printf("====================================" RESET_COLOR "\n");
    printf("Tests run: %d\n", test_count);
    printf("Tests passed: " GREEN_COLOR "%d" RESET_COLOR "\n", test_passed);
    printf("Tests failed: " RED_COLOR "%d" RESET_COLOR "\n", test_count - test_passed);
    
    if (test_passed == test_count) {
        printf(GREEN_COLOR "\n🎉 All tests passed!" RESET_COLOR "\n");
        return 0;
    } else {
        printf(RED_COLOR "\n❌ Some tests failed!" RESET_COLOR "\n");
        return 1;
    }
}