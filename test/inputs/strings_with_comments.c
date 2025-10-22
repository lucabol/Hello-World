/* Test file with strings containing comment-like sequences */
#include <stdio.h>

int main() {
    char* str1 = "This /* is not */ a comment";
    char* str2 = "This // is not a comment";
    printf("/* Still not a comment */\n");
    printf("// Also not a comment\n");
    return 0;
}
