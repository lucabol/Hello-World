#include <stdio.h>
#include "plugin.h"

int main(){
    const char* message = "Hello world!";
    const char* final_message = apply_plugins(message);
    printf("%s", final_message);
    return 0;
}
