#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define MAX_LINE_LENGTH 1024
#define BUFFER_SIZE 4096

/* Error codes */
typedef enum {
    SUCCESS = 0,
    ERROR_FILE_NOT_FOUND = 1,
    ERROR_PERMISSION_DENIED = 2,
    ERROR_READ_FAILURE = 3,
    ERROR_WRITE_FAILURE = 4,
    ERROR_INVALID_FORMAT = 5,
    ERROR_DISK_FULL = 6,
    ERROR_MEMORY_ALLOCATION = 7
} ErrorCode;

/* Structure to hold metrics */
typedef struct {
    int line_count;
    int char_count;
    int word_count;
} CodeMetrics;

/* Function prototypes */
ErrorCode analyze_file(const char *filename, CodeMetrics *metrics);
ErrorCode write_metrics(const char *output_file, const CodeMetrics *metrics);
void print_error_message(ErrorCode error, const char *context);
int is_valid_c_file(const char *filename);

/**
 * Print detailed error messages based on error code
 */
void print_error_message(ErrorCode error, const char *context) {
    switch (error) {
        case ERROR_FILE_NOT_FOUND:
            fprintf(stderr, "Error: File not found - %s\n", context);
            break;
        case ERROR_PERMISSION_DENIED:
            fprintf(stderr, "Error: Permission denied - %s\n", context);
            fprintf(stderr, "Please check file permissions and try again.\n");
            break;
        case ERROR_READ_FAILURE:
            fprintf(stderr, "Error: Failed to read from file - %s\n", context);
            fprintf(stderr, "The file may be corrupted or inaccessible.\n");
            break;
        case ERROR_WRITE_FAILURE:
            fprintf(stderr, "Error: Failed to write to file - %s\n", context);
            fprintf(stderr, "Check disk space and write permissions.\n");
            break;
        case ERROR_INVALID_FORMAT:
            fprintf(stderr, "Error: Invalid file format - %s\n", context);
            fprintf(stderr, "Expected a valid C source file (.c or .h extension).\n");
            break;
        case ERROR_DISK_FULL:
            fprintf(stderr, "Error: Disk full or insufficient space - %s\n", context);
            fprintf(stderr, "Free up disk space and try again.\n");
            break;
        case ERROR_MEMORY_ALLOCATION:
            fprintf(stderr, "Error: Memory allocation failed - %s\n", context);
            break;
        default:
            fprintf(stderr, "Error: Unknown error occurred - %s\n", context);
            break;
    }
}

/**
 * Validate if the file has a valid C source file extension
 */
int is_valid_c_file(const char *filename) {
    size_t len = strlen(filename);
    if (len < 3) {
        return 0;
    }
    return (strcmp(filename + len - 2, ".c") == 0) ||
           (strcmp(filename + len - 2, ".h") == 0);
}

/**
 * Analyze a file and compute basic code metrics
 * Returns error code indicating success or type of failure
 */
ErrorCode analyze_file(const char *filename, CodeMetrics *metrics) {
    FILE *file = NULL;
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    int in_word = 0;
    
    /* Initialize metrics */
    if (metrics == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    metrics->line_count = 0;
    metrics->char_count = 0;
    metrics->word_count = 0;
    
    /* Validate filename */
    if (filename == NULL || strlen(filename) == 0) {
        print_error_message(ERROR_FILE_NOT_FOUND, "No filename provided");
        return ERROR_FILE_NOT_FOUND;
    }
    
    /* Validate file extension */
    if (!is_valid_c_file(filename)) {
        print_error_message(ERROR_INVALID_FORMAT, filename);
        return ERROR_INVALID_FORMAT;
    }
    
    /* Attempt to open the file */
    file = fopen(filename, "r");
    if (file == NULL) {
        /* Determine specific error type */
        if (errno == ENOENT) {
            print_error_message(ERROR_FILE_NOT_FOUND, filename);
            return ERROR_FILE_NOT_FOUND;
        } else if (errno == EACCES || errno == EPERM) {
            print_error_message(ERROR_PERMISSION_DENIED, filename);
            return ERROR_PERMISSION_DENIED;
        } else {
            print_error_message(ERROR_READ_FAILURE, filename);
            return ERROR_READ_FAILURE;
        }
    }
    
    /* Read and analyze the file */
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            char c = buffer[i];
            metrics->char_count++;
            
            /* Count lines */
            if (c == '\n') {
                metrics->line_count++;
                in_word = 0;
            }
            
            /* Count words (simple space-delimited) */
            if (c == ' ' || c == '\t' || c == '\n') {
                in_word = 0;
            } else if (!in_word) {
                in_word = 1;
                metrics->word_count++;
            }
        }
    }
    
    /* Check for read errors */
    if (ferror(file)) {
        print_error_message(ERROR_READ_FAILURE, filename);
        fclose(file);
        return ERROR_READ_FAILURE;
    }
    
    /* Close the file */
    if (fclose(file) != 0) {
        print_error_message(ERROR_READ_FAILURE, filename);
        return ERROR_READ_FAILURE;
    }
    
    return SUCCESS;
}

/**
 * Write metrics to an output file
 * Returns error code indicating success or type of failure
 */
ErrorCode write_metrics(const char *output_file, const CodeMetrics *metrics) {
    FILE *file = NULL;
    
    /* Validate inputs */
    if (output_file == NULL || metrics == NULL) {
        return ERROR_WRITE_FAILURE;
    }
    
    /* Attempt to open output file for writing */
    file = fopen(output_file, "w");
    if (file == NULL) {
        /* Determine specific error type */
        if (errno == EACCES || errno == EPERM) {
            print_error_message(ERROR_PERMISSION_DENIED, output_file);
            return ERROR_PERMISSION_DENIED;
        } else if (errno == ENOSPC) {
            print_error_message(ERROR_DISK_FULL, output_file);
            return ERROR_DISK_FULL;
        } else {
            print_error_message(ERROR_WRITE_FAILURE, output_file);
            return ERROR_WRITE_FAILURE;
        }
    }
    
    /* Write metrics to file */
    if (fprintf(file, "Code Metrics Report\n") < 0) {
        print_error_message(ERROR_WRITE_FAILURE, output_file);
        fclose(file);
        return ERROR_WRITE_FAILURE;
    }
    if (fprintf(file, "===================\n") < 0) {
        print_error_message(ERROR_WRITE_FAILURE, output_file);
        fclose(file);
        return ERROR_WRITE_FAILURE;
    }
    if (fprintf(file, "Lines: %d\n", metrics->line_count) < 0) {
        print_error_message(ERROR_WRITE_FAILURE, output_file);
        fclose(file);
        return ERROR_WRITE_FAILURE;
    }
    if (fprintf(file, "Characters: %d\n", metrics->char_count) < 0) {
        print_error_message(ERROR_WRITE_FAILURE, output_file);
        fclose(file);
        return ERROR_WRITE_FAILURE;
    }
    if (fprintf(file, "Words: %d\n", metrics->word_count) < 0) {
        print_error_message(ERROR_WRITE_FAILURE, output_file);
        fclose(file);
        return ERROR_WRITE_FAILURE;
    }
    
    /* Flush to ensure data is written */
    if (fflush(file) != 0) {
        if (errno == ENOSPC) {
            print_error_message(ERROR_DISK_FULL, output_file);
            fclose(file);
            return ERROR_DISK_FULL;
        } else {
            print_error_message(ERROR_WRITE_FAILURE, output_file);
            fclose(file);
            return ERROR_WRITE_FAILURE;
        }
    }
    
    /* Close the file and check for errors */
    if (fclose(file) != 0) {
        if (errno == ENOSPC) {
            print_error_message(ERROR_DISK_FULL, output_file);
            return ERROR_DISK_FULL;
        } else {
            print_error_message(ERROR_WRITE_FAILURE, output_file);
            return ERROR_WRITE_FAILURE;
        }
    }
    
    return SUCCESS;
}

/**
 * Main function - demonstrates usage of the metrics analyzer
 */
int main(int argc, char *argv[]) {
    CodeMetrics metrics;
    ErrorCode result;
    
    /* Check command line arguments */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file> [output_file]\n", argv[0]);
        fprintf(stderr, "  input_file:  C source file to analyze (.c or .h)\n");
        fprintf(stderr, "  output_file: Optional output file for metrics report\n");
        return EXIT_FAILURE;
    }
    
    /* Analyze the input file */
    result = analyze_file(argv[1], &metrics);
    
    if (result != SUCCESS) {
        fprintf(stderr, "Analysis failed with error code: %d\n", result);
        return EXIT_FAILURE;
    }
    
    /* Print metrics to stdout */
    printf("Code Metrics for: %s\n", argv[1]);
    printf("Lines: %d\n", metrics.line_count);
    printf("Characters: %d\n", metrics.char_count);
    printf("Words: %d\n", metrics.word_count);
    
    /* Write to output file if specified */
    if (argc >= 3) {
        result = write_metrics(argv[2], &metrics);
        if (result != SUCCESS) {
            fprintf(stderr, "Failed to write metrics to file\n");
            return EXIT_FAILURE;
        }
        printf("Metrics written to: %s\n", argv[2]);
    }
    
    return EXIT_SUCCESS;
}
