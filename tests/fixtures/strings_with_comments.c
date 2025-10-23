#include <stdio.h>

int main() {
    char *s1 = "This // is not a comment";
    char *s2 = "Neither /* is */ this";
    char c = '/'; // This IS a comment
    printf("%s\n", s1);
    return 0;
}
