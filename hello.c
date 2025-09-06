/**
 * @file hello.c
 * @brief A simple "Hello World" program in C
 * @description This program demonstrates basic C programming by printing
 *              a greeting message to standard output.
 * @author Refactored for improved readability
 */

#include <stdio.h>

/**
 * @brief Main entry point of the program
 * @return int Exit status code (0 for success)
 */
int main(void) {
    // Define the greeting message for better maintainability
    const char* greeting_message = "Ciao, Mondo!";
    
    // Print the greeting message to standard output
    printf("%s\n", greeting_message);
    
    // Explicitly return success status
    return 0;
}
