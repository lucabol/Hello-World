#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    // Demonstrate memory allocation with error handling
    char *message = malloc(50 * sizeof(char));
    if (message == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    // Safely copy string with bounds checking
    strncpy(message, "Hello world!", 49);
    message[49] = '\0';  // Ensure null termination
    
    // Check if printf() succeeds
    if (printf("%s\n", message) < 0) {
        fprintf(stderr, "Error: Failed to print message\n");
        free(message);  // Clean up before exit
        return EXIT_FAILURE;
    }
    
    // Demonstrate file operation error handling
    FILE *temp_file = fopen("/tmp/hello_test.txt", "w");
    if (temp_file == NULL) {
        fprintf(stderr, "Warning: Could not create temporary file\n");
        // Continue execution - this is non-critical
    } else {
        if (fprintf(temp_file, "Hello from file!\n") < 0) {
            fprintf(stderr, "Warning: Failed to write to file\n");
        }
        if (fclose(temp_file) != 0) {
            fprintf(stderr, "Warning: Failed to close file properly\n");
        }
    }
    
    // Print blank line and exit code as expected
    if (printf("\nExit code: 0\n") < 0) {
        fprintf(stderr, "Error: Failed to print exit message\n");
        free(message);
        return EXIT_FAILURE;
    }
    
    // Clean up allocated memory
    free(message);
    return EXIT_SUCCESS;
}
