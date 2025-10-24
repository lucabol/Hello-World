/* Test file with edge cases for code_metrics analyzer
 * This file contains various edge cases to test the robustness
 * of the analyzer
 */

#include <stdio.h>
#include <string.h>

/* Multi-line comment
 * with several lines
 * to test comment counting
 */

// Single line comment

int main() {
    /* Inline comment */ int x = 5;
    
    // String with comment-like content
    char *str = "This /* is not */ a comment";
    char *str2 = "Neither is // this";
    
    // Printf in string vs actual printf
    printf("The word printf appears here\n");
    char *word = "printf";  // This printf should not be counted
    
    /* Multi-line
       comment that
       spans several lines */
    
    return 0;
}

// Function definition patterns
int simple_function(int a) {
    return a * 2;
}

void no_return_value(void) {
    printf("Testing void functions\n");
}

char get_char(void) {
    return 'a';
}

float calculate(float x, float y) {
    return x + y;
}

double precise_calc(double val) {
    return val * 3.14159;
}

/* This is a prototype, not a definition */
// int prototype_function(int x);

/* Very long line to test max line length tracking: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
