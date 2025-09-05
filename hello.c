# include <stdio.h>

void print_message(const char* message) {
    printf("%s\n", message);
}

int main(){
    print_message("Ciao, Mondo!");
    printf("\n");
    printf("Exit code: %d\n", 0);
    return 0;
}