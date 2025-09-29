# include <stdio.h>

/*
 * Hello World Program
 * 
 * By default, outputs "Ciao, Mondo!" without a trailing newline.
 * This preserves the original specification but may cause terminal display issues.
 * 
 * To build with a trailing newline for better terminal compatibility:
 *   gcc -DWITH_NEWLINE -o hello_newline hello.c
 * 
 * Or use the Makefile:
 *   make          # Default version (no newline)
 *   make newline  # Version with trailing newline
 */
int main(){
    printf("Ciao, Mondo!");
#ifdef WITH_NEWLINE
    printf("\n");
#endif
    return 0;
}
