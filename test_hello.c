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

void test_hello_output_consistency() {
    char output1[256], output2[256], output3[256];
    int exit_code1, exit_code2, exit_code3;
    
    // Run multiple times to verify consistency
    exit_code1 = run_hello_and_capture(output1, sizeof(output1));
    exit_code2 = run_hello_and_capture(output2, sizeof(output2));
    exit_code3 = run_hello_and_capture(output3, sizeof(output3));
    
    assert_equal_string(output1, output2, "Multiple runs produce consistent output (run 1 vs 2)");
    assert_equal_string(output2, output3, "Multiple runs produce consistent output (run 2 vs 3)");
    assert_equal_int(exit_code1, exit_code2, "Multiple runs produce consistent exit code (run 1 vs 2)");
    assert_equal_int(exit_code2, exit_code3, "Multiple runs produce consistent exit code (run 2 vs 3)");
}

void test_hello_output_format() {
    char output[256];
    run_hello_and_capture(output, sizeof(output));
    
    // Test specific format requirements
    assert_equal_int(12, (int)strlen(output), "Output has correct length (12 characters)");
    
    // Verify no newline at end (based on hello.c implementation)
    test_count++;
    if (output[strlen(output)] == '\0' && (strlen(output) == 0 || output[strlen(output)-1] != '\n')) {
        printf("PASS: Output has no trailing newline\n");
        test_passed++;
    } else {
        printf("FAIL: Output has unexpected trailing newline\n");
    }
    
    // Verify first character
    test_count++;
    if (output[0] == 'H') {
        printf("PASS: Output starts with 'H'\n");
        test_passed++;
    } else {
        printf("FAIL: Output does not start with 'H', got '%c'\n", output[0]);
    }
    
    // Verify last character
    test_count++;
    if (strlen(output) > 0 && output[strlen(output)-1] == '!') {
        printf("PASS: Output ends with '!'\n");
        test_passed++;
    } else {
        printf("FAIL: Output does not end with '!'\n");
    }
}

void test_hello_edge_cases() {
    char small_buffer[5];  // Deliberately small buffer
    int exit_code;
    
    // Test with small buffer (should still work, just truncated)
    exit_code = run_hello_and_capture(small_buffer, sizeof(small_buffer));
    assert_equal_int(0, exit_code, "Hello program exits correctly even with small capture buffer");
    
    // Verify truncated output
    assert_equal_string("Hell", small_buffer, "Small buffer captures truncated output correctly");
}

int test_missing_executable() {
    // This is tested in main() but we'll add explicit validation
    test_count++;
    if (access("./hello", X_OK) == 0) {
        printf("PASS: Hello executable exists and is executable\n");
        test_passed++;
        return 1;
    } else {
        printf("FAIL: Hello executable missing or not executable\n");
        return 0;
    }
}

void test_negative_scenarios() {
    // Test 1: Backup the original hello and test missing executable
    char output[256];
    int exit_code;
    
    // First verify normal operation
    exit_code = run_hello_and_capture(output, sizeof(output));
    test_count++;
    if (exit_code == 0) {
        printf("PASS: Baseline test - hello executes successfully\n");
        test_passed++;
    } else {
        printf("FAIL: Baseline test failed - hello should execute successfully\n");
    }
    
    // Test 2: Create a corrupted/non-executable file temporarily
    system("cp hello hello_backup");  // Backup original
    system("echo 'corrupted' > hello_corrupted");
    system("mv hello_corrupted hello");  // Replace with corrupted version
    
    exit_code = run_hello_and_capture(output, sizeof(output));
    test_count++;
    if (exit_code != 0) {
        printf("PASS: Corrupted executable properly fails\n");
        test_passed++;
    } else {
        printf("FAIL: Corrupted executable should have failed\n");
    }
    
    // Test 3: Test with missing executable
    system("rm -f hello");
    exit_code = run_hello_and_capture(output, sizeof(output));
    test_count++;
    if (exit_code != 0) {
        printf("PASS: Missing executable properly fails\n");
        test_passed++;
    } else {
        printf("FAIL: Missing executable should have failed\n");
    }
    
    // Restore original
    system("mv hello_backup hello");
    
    // Verify restoration worked
    exit_code = run_hello_and_capture(output, sizeof(output));
    test_count++;
    if (exit_code == 0 && strcmp(output, "Hello world!") == 0) {
        printf("PASS: Executable restoration successful\n");
        test_passed++;
    } else {
        printf("FAIL: Failed to restore executable properly\n");
    }
}

void test_boundary_conditions() {
    char tiny_buffer[2];
    char large_buffer[1024];
    int exit_code;
    
    // Test with minimal buffer
    exit_code = run_hello_and_capture(tiny_buffer, sizeof(tiny_buffer));
    assert_equal_int(0, exit_code, "Program succeeds even with tiny capture buffer");
    assert_equal_string("H", tiny_buffer, "Tiny buffer captures first character");
    
    // Test with large buffer  
    exit_code = run_hello_and_capture(large_buffer, sizeof(large_buffer));
    assert_equal_int(0, exit_code, "Program succeeds with large capture buffer");
    assert_equal_string("Hello world!", large_buffer, "Large buffer captures complete output");
    
    // Verify large buffer doesn't have extra content
    test_count++;
    if (strlen(large_buffer) == 12) {
        printf("PASS: Large buffer contains exactly expected content\n");
        test_passed++;
    } else {
        printf("FAIL: Large buffer has unexpected length: %d\n", (int)strlen(large_buffer));
    }
}

void test_hello_resource_handling() {
    // Test multiple rapid executions to check for resource leaks
    char output[256];
    int exit_code;
    int successful_runs = 0;
    
    for (int i = 0; i < 10; i++) {
        exit_code = run_hello_and_capture(output, sizeof(output));
        if (exit_code == 0 && strcmp(output, "Hello world!") == 0) {
            successful_runs++;
        }
    }
    
    assert_equal_int(10, successful_runs, "Multiple rapid executions succeed (no resource leaks)");
}

void test_hello_concurrent_execution() {
    // Test concurrent execution by forking test processes
    pid_t pids[3];
    int status;
    int successful_children = 0;
    
    for (int i = 0; i < 3; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            // Child process - run a quick test
            char output[256];
            int exit_code = run_hello_and_capture(output, sizeof(output));
            if (exit_code == 0 && strcmp(output, "Hello world!") == 0) {
                exit(0);  // Success
            } else {
                exit(1);  // Failure
            }
        } else if (pids[i] < 0) {
            perror("fork failed in concurrent test");
        }
    }
    
    // Wait for all children and count successes
    for (int i = 0; i < 3; i++) {
        if (pids[i] > 0) {
            waitpid(pids[i], &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                successful_children++;
            }
        }
    }
    
    assert_equal_int(3, successful_children, "Concurrent execution works correctly");
}

int main() {
    printf("Running comprehensive tests for hello.c...\n\n");
    
    /* Check if hello executable exists */
    if (access("./hello", X_OK) != 0) {
        printf("ERROR: ./hello executable not found or not executable.\n");
        printf("Please compile with: gcc -o hello hello.c\n");
        return 1;
    }
    
    printf("=== Basic Functionality Tests ===\n");
    test_hello_output();
    
    printf("\n=== Consistency Tests ===\n");
    test_hello_output_consistency();
    
    printf("\n=== Output Format Tests ===\n");
    test_hello_output_format();
    
    printf("\n=== Edge Case Tests ===\n");
    test_hello_edge_cases();
    
    printf("\n=== Boundary Condition Tests ===\n");
    test_boundary_conditions();
    
    printf("\n=== Resource Handling Tests ===\n");
    test_hello_resource_handling();
    
    printf("\n=== Concurrent Execution Tests ===\n");
    test_hello_concurrent_execution();
    
    printf("\n=== Negative Scenario Tests ===\n");
    test_negative_scenarios();
    
    printf("\n=== Final Validation ===\n");
    test_missing_executable();
    
    printf("\n" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "\n");
    printf("Test Results Summary:\n");
    printf("Tests run: %d\n", test_count);
    printf("Tests passed: %d\n", test_passed);
    printf("Tests failed: %d\n", test_count - test_passed);
    
    if (test_passed == test_count) {
        printf("\n🎉 All tests PASSED! 🎉\n");
        return 0;
    } else {
        printf("\n❌ Some tests FAILED! ❌\n");
        return 1;
    }
}