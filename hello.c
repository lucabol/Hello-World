#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    int add_newline = 0;
    
    // Check for -n flag (only accepts exactly "./hello -n", other arguments are ignored)
    if (argc == 2 && strcmp(argv[1], "-n") == 0) {
        add_newline = 1;
    }
    
    printf("Hello world!");
    
    if (add_newline) {
        printf("\n");
    }
    
    return 0;
}
