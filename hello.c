#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>

#define _(String) gettext(String)
#define PACKAGE "hello"
#define LOCALEDIR "/home/runner/work/Hello-World/Hello-World/locale"

int main(int argc, char *argv[]) {
    // Parse command-line arguments for language override
    char *lang = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--lang") == 0 && i + 1 < argc) {
            lang = argv[i + 1];
            i++;
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
}
