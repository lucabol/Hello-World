/* Include the standard input/output library for puts() function */
# include <stdio.h>

/* 
 * Simple Hello World program
 * 
 * This program demonstrates basic C programming by displaying a greeting message.
 * It showcases the use of the puts() function for simple string output.
 */

/* 
 * main() - Entry point of the program
 * 
 * This is the main function where program execution begins.
 * Every C program must have exactly one main() function.
 * 
 * Return: 0 on successful execution (standard convention)
 */
int main(){
    /* 
     * Output the greeting message using puts()
     * 
     * Using puts() instead of printf() for simple string output because:
     * - puts() automatically adds a newline character (\n) at the end
     * - puts() is more efficient for outputting plain strings without formatting
     * - puts() is safer as it doesn't process format specifiers
     * - puts() has better performance for simple string literals
     */
    puts("Hello, World!");
 
    /* 
     * Return 0 to indicate successful program termination
     * 
     * In C, returning 0 from main() indicates that the program
     * executed successfully without errors. Non-zero values
     * typically indicate various error conditions.
     */
    return 0;
}
