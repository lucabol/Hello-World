# include <stdio.h>
# include <stdlib.h>
# include <string.h>

int main(int argc, char *argv[]){
    int add_newline = 0;
    
    // Check for command-line flag: -n or --newline
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--newline") == 0) {
            add_newline = 1;
            break;
        }
    }
    
    // Check for environment variable HELLO_NEWLINE
    if (!add_newline) {
        char *env_newline = getenv("HELLO_NEWLINE");
        if (env_newline != NULL && 
            (strcmp(env_newline, "1") == 0 || strcmp(env_newline, "true") == 0)) {
            add_newline = 1;
        }
    }
    
    printf("Hello world!");
    if (add_newline) {
        printf("\n");
    }
    
    return 0;
}
