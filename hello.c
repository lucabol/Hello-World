# include <stdio.h>

void print_message(const char* message) {
    printf("%s", message);
}

int main(){
    print_message("Hello world!");
    return 0;
}
