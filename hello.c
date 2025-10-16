#include <stdio.h>
#include "hello.h"

const char* get_greeting(void){
    return "Hello world!";
}

void print_message(const char* msg){
    printf("%s", msg);
}

#ifndef UNIT_TEST
int main(){
    print_message(get_greeting());
    return 0;
}
#endif
