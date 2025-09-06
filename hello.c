# include <stdio.h>

void print_custom_message(const char* message) {
    printf("%s", message);
}

int main(){
    print_custom_message("Hello world!\n");
    return 0;
}
