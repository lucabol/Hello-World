# include <stdio.h>
#include "hello.h"
#include "plugin.h"

const char* get_greeting(void) {
    return "Hello world!";
}

int main(){
    const char* greeting = get_greeting();
    const char* transformed = apply_plugins(greeting);
    printf("%s", transformed);
}
