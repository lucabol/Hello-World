#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
#include <string.h>
#include <locale.h>
#include <libintl.h>

#define _(String) gettext(String)
#define PACKAGE "hello"

// LOCALEDIR can be overridden at compile time with -DLOCALEDIR=...
#ifndef LOCALEDIR
#define LOCALEDIR "./locale"
#endif

int main(int argc, char *argv[]) {
    // Parse command-line arguments for language override
    char *lang = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--lang") == 0) {
            if (i + 1 < argc) {
                lang = argv[i + 1];
                i++;
            } else {
                fprintf(stderr, "Error: --lang requires a language argument\n");
                fprintf(stderr, "Usage: %s [--lang LOCALE]\n", argv[0]);
                fprintf(stderr, "Example: %s --lang es_ES.UTF-8\n", argv[0]);
                return 1;
            }
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Usage: %s [--lang LOCALE]\n", argv[0]);
            printf("Print a greeting message in the specified language.\n\n");
            printf("Options:\n");
            printf("  --lang LOCALE  Set the language (e.g., es_ES.UTF-8, fr_FR.UTF-8)\n");
            printf("  --help, -h     Show this help message\n\n");
            printf("Supported languages:\n");
            printf("  en (default)   English\n");
            printf("  es_ES.UTF-8    Spanish\n");
            printf("  fr_FR.UTF-8    French\n");
            printf("  de_DE.UTF-8    German\n");
            printf("  ja_JP.UTF-8    Japanese\n");
            return 0;
        } else {
            fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
            fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
            return 1;
        }
    }
    
    // Set language from command-line argument
    if (lang != NULL) {
        setenv("LANG", lang, 1);
    }
    
    // Initialize locale system - force UTF-8 if needed
    char *locale_result = setlocale(LC_ALL, "");
    if (locale_result == NULL || strcmp(locale_result, "C") == 0 || strcmp(locale_result, "POSIX") == 0) {
        // Try to set a UTF-8 locale
        setlocale(LC_ALL, "C.UTF-8");
    }
    
    bindtextdomain(PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(PACKAGE, "UTF-8");
    textdomain(PACKAGE);
    
    // Print the message - gettext will translate if available
    printf("%s", _("Hello world!"));
    
    return 0;
=======

/* Main function to print greeting */
#ifndef UNIT_TEST
int main(){
#else
int hello_main(){
#endif
    if (printf("Hello world!") < 0) {
        fprintf(stderr, "Error: Failed to write output\n");
        return EXIT_FAILURE;
    }
    if (fflush(stdout) != 0) {
        fprintf(stderr, "Error: Failed to flush output\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
>>>>>>> main
}
