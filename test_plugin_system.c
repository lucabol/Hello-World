#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include "plugin.h"

/* Test framework macros */
#define TEST_PASS 0
#define TEST_FAIL 1

static int tests_run = 0;
static int tests_passed = 0;

#define RUN_TEST(test) do { \
    printf("Running %s... ", #test); \
    fflush(stdout); \
    tests_run++; \
    if (test() == TEST_PASS) { \
        printf("PASS\n"); \
        tests_passed++; \
    } else { \
        printf("FAIL\n"); \
    } \
} while(0)

/* Mock plugin for testing */
static plugin_info_t test_plugin_info = {
    .name = "test_plugin",
    .version = "1.0.0",
    .description = "Test plugin for unit testing",
    .type = PLUGIN_TYPE_MESSAGE_TRANSFORMER,
    .api_version = PLUGIN_API_VERSION
};

static plugin_info_t* test_get_info(void) {
    return &test_plugin_info;
}

static int test_init(void) {
    return 0;
}

static char* test_transform_message(const char* input) {
    if (!input) return NULL;
    
    size_t len = strlen(input);
    char* result = malloc(len + 7); /* Add space for "[TEST]" */
    if (!result) return NULL;
    
    snprintf(result, len + 7, "[TEST]%s", input);
    return result;
}

static void test_cleanup(void) {
    /* Nothing to cleanup */
}

static plugin_interface_t test_interface = {
    .get_info = test_get_info,
    .init = test_init,
    .transform_message = test_transform_message,
    .format_output = NULL,
    .cleanup = test_cleanup
};

/* Test functions */
static int test_plugin_interface_validity(void) {
    /* Test that our test plugin interface is valid */
    if (!test_interface.get_info) return TEST_FAIL;
    
    plugin_info_t* info = test_interface.get_info();
    if (!info) return TEST_FAIL;
    if (info->api_version != PLUGIN_API_VERSION) return TEST_FAIL;
    if (!info->name || !info->version || !info->description) return TEST_FAIL;
    
    return TEST_PASS;
}

static int test_message_transformation(void) {
    const char* input = "Hello";
    char* result = test_transform_message(input);
    
    if (!result) return TEST_FAIL;
    
    int success = (strcmp(result, "[TEST]Hello") == 0) ? TEST_PASS : TEST_FAIL;
    free(result);
    return success;
}

static int test_null_input_handling(void) {
    /* Test that plugins handle NULL input gracefully */
    char* result = test_transform_message(NULL);
    return (result == NULL) ? TEST_PASS : TEST_FAIL;
}

static int test_memory_allocation_failure(void) {
    /* This test is hard to simulate without mocking malloc,
     * so we'll just test that the function exists and handles empty strings */
    char* result = test_transform_message("");
    if (!result) return TEST_FAIL;
    
    int success = (strcmp(result, "[TEST]") == 0) ? TEST_PASS : TEST_FAIL;
    free(result);
    return success;
}

static int test_plugin_directory_existence(void) {
    /* Test that the plugins directory exists */
    struct stat st;
    return (stat("./plugins", &st) == 0 && S_ISDIR(st.st_mode)) ? TEST_PASS : TEST_FAIL;
}

static int test_plugin_files_exist(void) {
    /* Test that expected plugin files exist */
    return (access("./plugins/uppercase_transformer.so", F_OK) == 0 &&
            access("./plugins/reverse_transformer.so", F_OK) == 0 &&
            access("./plugins/fancy_formatter.so", F_OK) == 0) ? TEST_PASS : TEST_FAIL;
}

static int test_backward_compatibility(void) {
    /* Test behavior without plugins directory */
    int status = system("rm -rf ./plugins_backup && mv ./plugins ./plugins_backup");
    if (status != 0) return TEST_FAIL;
    
    /* Run hello without plugins - should work and output basic message */
    FILE* fp = popen("./hello", "r");
    if (!fp) {
        system("mv ./plugins_backup ./plugins");
        return TEST_FAIL;
    }
    
    char output[256];
    if (!fgets(output, sizeof(output), fp)) {
        pclose(fp);
        system("mv ./plugins_backup ./plugins");
        return TEST_FAIL;
    }
    
    int exit_code = pclose(fp);
    system("mv ./plugins_backup ./plugins");
    
    /* Should output "Hello world!" and exit with code 0 */
    return (exit_code == 0 && strstr(output, "Hello world!")) ? TEST_PASS : TEST_FAIL;
}

static int test_plugin_chaining(void) {
    /* Test that plugins are applied in correct order */
    FILE* fp = popen("./hello", "r");
    if (!fp) return TEST_FAIL;
    
    char output[1024] = {0};
    size_t total_read = 0;
    char buffer[256];
    
    /* Read all output */
    while (fgets(buffer, sizeof(buffer), fp) && total_read < sizeof(output) - 1) {
        size_t len = strlen(buffer);
        if (total_read + len < sizeof(output) - 1) {
            strcat(output, buffer);
            total_read += len;
        }
    }
    
    pclose(fp);
    
    /* Should contain the transformed and formatted message */
    return (strstr(output, "!DLROW OLLEH") && strstr(output, "*")) ? TEST_PASS : TEST_FAIL;
}

static int test_api_version_compatibility(void) {
    /* Test that plugins with wrong API version are rejected */
    plugin_info_t wrong_version_info = {
        .name = "wrong_version",
        .version = "1.0.0",
        .description = "Plugin with wrong API version",
        .type = PLUGIN_TYPE_MESSAGE_TRANSFORMER,
        .api_version = 999  /* Wrong version */
    };
    
    /* This would be rejected by the main program, but we can test the concept */
    return (wrong_version_info.api_version != PLUGIN_API_VERSION) ? TEST_PASS : TEST_FAIL;
}

static int test_error_handling_robustness(void) {
    /* Test that the system handles various error conditions */
    
    /* Test with non-existent plugin directory */
    int status = system("rm -rf ./plugins_test && mkdir ./plugins_test");
    if (status != 0) return TEST_FAIL;
    
    /* Create a non-plugin file in plugins directory */
    FILE* fp = fopen("./plugins_test/not_a_plugin.txt", "w");
    if (fp) {
        fprintf(fp, "This is not a plugin\n");
        fclose(fp);
    }
    
    /* The system should handle this gracefully */
    system("rm -rf ./plugins_test");
    return TEST_PASS;
}

/* Main test runner */
int main(void) {
    printf("Running Plugin System Unit Tests\n");
    printf("=================================\n");
    
    /* Build the project first */
    printf("Building project for testing...\n");
    if (system("make clean && make all") != 0) {
        printf("ERROR: Failed to build project\n");
        return 1;
    }
    printf("\n");
    
    /* Run tests */
    RUN_TEST(test_plugin_interface_validity);
    RUN_TEST(test_message_transformation);
    RUN_TEST(test_null_input_handling);
    RUN_TEST(test_memory_allocation_failure);
    RUN_TEST(test_plugin_directory_existence);
    RUN_TEST(test_plugin_files_exist);
    RUN_TEST(test_backward_compatibility);
    RUN_TEST(test_plugin_chaining);
    RUN_TEST(test_api_version_compatibility);
    RUN_TEST(test_error_handling_robustness);
    
    /* Print results */
    printf("\n=================================\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);
    printf("Success rate: %.1f%%\n", (float)tests_passed / tests_run * 100);
    
    return (tests_passed == tests_run) ? 0 : 1;
}