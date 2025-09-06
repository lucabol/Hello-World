# include <stdio.h>

void print_custom_message(const char* message) {
    printf("%s", message);
}

void run_tests() {
    printf("Running tests for print_custom_message():\n");
    
    // Test 1: Normal message
    printf("Test 1 (normal message): ");
    print_custom_message("Hello world!\n");
    
    // Test 2: Empty message
    printf("Test 2 (empty message): ");
    print_custom_message("");
    printf("[empty message test completed]\n");
    
    // Test 3: Long message
    printf("Test 3 (long message): ");
    print_custom_message("This is a very long message to test that the function can handle longer strings without any issues.\n");
    
    // Test 4: Special characters
    printf("Test 4 (special chars): ");
    print_custom_message("Special chars: !@#$%^&*()_+-=[]{}|;':\",./<>?\n");
    
    // Test 5: Numbers
    printf("Test 5 (numbers): ");
    print_custom_message("Numbers: 1234567890\n");
    
    printf("All tests completed.\n\n");
}

int main(){
#ifdef RUN_TESTS
    // Run tests when compiled with -DRUN_TESTS
    run_tests();
#endif
    
    // Original functionality
    print_custom_message("Hello world!\n");
    return 0;
}
