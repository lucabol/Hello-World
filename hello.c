# include <stdio.h>

void print_message(const char* message){
    printf("%s\n\n", message);
}
 
int main(){
    print_message("Ciao, Mondo!");
 
    int exit_code = 0;
    printf("Exit code: %d\n", exit_code);
    return exit_code;
}
