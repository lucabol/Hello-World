# include <stdio.h>

void print_message(const char* message){
    printf("%s\n", message);
}

int main(void){
    print_message("Hello world!");
    return 0;
}
