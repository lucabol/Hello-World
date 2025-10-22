#include "metrics.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    const char* filename = "hello.c";
    int interactive = 0;
    
    /* Parse command line arguments */
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interactive") == 0) {
                interactive = 1;
            } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                printf("Code Metrics Spreadsheet Tool\n");
                printf("==============================\n");
                printf("\n");
                printf("Usage: %s [options] [filename]\n", argv[0]);
                printf("\n");
                printf("Options:\n");
                printf("  -i, --interactive    Launch interactive menu\n");
                printf("  -h, --help          Show this help message\n");
                printf("\n");
                printf("Default file: hello.c\n");
                printf("\n");
                return 0;
            } else {
                filename = argv[i];
            }
        }
    }
    
    /* Analyze the file */
    CodeMetrics* metrics = analyze_file(filename);
    
    if (!metrics) {
        fprintf(stderr, "Error: Could not analyze file '%s'\n", filename);
        fprintf(stderr, "Please ensure the file exists and is readable.\n");
        return 1;
    }
    
    /* Display results */
    if (interactive) {
        interactive_menu(metrics, filename);
    } else {
        display_metrics_spreadsheet(metrics, filename);
    }
    
    /* Clean up */
    free_metrics(metrics);
    
    return 0;
}
