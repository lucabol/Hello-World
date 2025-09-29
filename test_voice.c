#include "voice.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Test helper macros
#define ASSERT_EQ(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            printf("FAIL: %s:%d: Expected %d, got %d\n", __FILE__, __LINE__, (expected), (actual)); \
            return 1; \
        } \
    } while(0)

#define ASSERT_STR_EQ(actual, expected) \
    do { \
        if (strcmp((actual), (expected)) != 0) { \
            printf("FAIL: %s:%d: Expected '%s', got '%s'\n", __FILE__, __LINE__, (expected), (actual)); \
            return 1; \
        } \
    } while(0)

#define ASSERT_NULL(ptr) \
    do { \
        if ((ptr) != NULL) { \
            printf("FAIL: %s:%d: Expected NULL pointer\n", __FILE__, __LINE__); \
            return 1; \
        } \
    } while(0)

#define ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == NULL) { \
            printf("FAIL: %s:%d: Expected non-NULL pointer\n", __FILE__, __LINE__); \
            return 1; \
        } \
    } while(0)

// Test parsing of basic commands
int test_parse_basic_commands(void) {
    printf("Testing basic command parsing...\n");
    
    // Test "say hello" command
    voice_command_t* cmd = parse_voice_command("say hello");
    ASSERT_NOT_NULL(cmd);
    ASSERT_EQ(cmd->type, VOICE_CMD_SAY_HELLO);
    ASSERT_NULL(cmd->parameter);
    free_voice_command(cmd);
    
    // Test "show code" command
    cmd = parse_voice_command("show code");
    ASSERT_NOT_NULL(cmd);
    ASSERT_EQ(cmd->type, VOICE_CMD_SHOW_CODE);
    ASSERT_NULL(cmd->parameter);
    free_voice_command(cmd);
    
    printf("✓ Basic command parsing tests passed\n");
    return 0;
}

// Test parsing of commands with parameters
int test_parse_parameterized_commands(void) {
    printf("Testing parameterized command parsing...\n");
    
    // Test "change message" command
    voice_command_t* cmd = parse_voice_command("change message Hello World");
    ASSERT_NOT_NULL(cmd);
    ASSERT_EQ(cmd->type, VOICE_CMD_CHANGE_MESSAGE);
    ASSERT_NOT_NULL(cmd->parameter);
    ASSERT_STR_EQ(cmd->parameter, "Hello World");
    free_voice_command(cmd);
    
    // Test "insert code" command
    cmd = parse_voice_command("insert code // This is a comment");
    ASSERT_NOT_NULL(cmd);
    ASSERT_EQ(cmd->type, VOICE_CMD_INSERT_CODE);
    ASSERT_NOT_NULL(cmd->parameter);
    ASSERT_STR_EQ(cmd->parameter, "// This is a comment");
    free_voice_command(cmd);
    
    // Test "navigate to" command
    cmd = parse_voice_command("navigate to main");
    ASSERT_NOT_NULL(cmd);
    ASSERT_EQ(cmd->type, VOICE_CMD_NAVIGATE_TO);
    ASSERT_NOT_NULL(cmd->parameter);
    ASSERT_STR_EQ(cmd->parameter, "main");
    free_voice_command(cmd);
    
    // Test "refactor" command
    cmd = parse_voice_command("refactor function main");
    ASSERT_NOT_NULL(cmd);
    ASSERT_EQ(cmd->type, VOICE_CMD_REFACTOR);
    ASSERT_NOT_NULL(cmd->parameter);
    ASSERT_STR_EQ(cmd->parameter, "function main");
    free_voice_command(cmd);
    
    printf("✓ Parameterized command parsing tests passed\n");
    return 0;
}

// Test edge cases and error conditions
int test_parse_edge_cases(void) {
    printf("Testing edge cases and error conditions...\n");
    
    // Test NULL input
    voice_command_t* cmd = parse_voice_command(NULL);
    ASSERT_NULL(cmd);
    
    // Test empty string
    cmd = parse_voice_command("");
    ASSERT_NULL(cmd);
    
    // Test whitespace only
    cmd = parse_voice_command("   ");
    ASSERT_NOT_NULL(cmd);
    ASSERT_EQ(cmd->type, VOICE_CMD_UNKNOWN);
    ASSERT_NOT_NULL(cmd->parameter);
    ASSERT_STR_EQ(cmd->parameter, "   ");
    free_voice_command(cmd);
    
    // Test unknown command
    cmd = parse_voice_command("invalid command");
    ASSERT_NOT_NULL(cmd);
    ASSERT_EQ(cmd->type, VOICE_CMD_UNKNOWN);
    ASSERT_NOT_NULL(cmd->parameter);
    ASSERT_STR_EQ(cmd->parameter, "invalid command");
    free_voice_command(cmd);
    
    // Test commands with extra spaces
    cmd = parse_voice_command("change message    Hello   World");
    ASSERT_NOT_NULL(cmd);
    ASSERT_EQ(cmd->type, VOICE_CMD_CHANGE_MESSAGE);
    ASSERT_NOT_NULL(cmd->parameter);
    ASSERT_STR_EQ(cmd->parameter, "Hello   World");
    free_voice_command(cmd);
    
    // Test command without parameter where parameter expected
    cmd = parse_voice_command("change message");
    ASSERT_NOT_NULL(cmd);
    ASSERT_EQ(cmd->type, VOICE_CMD_CHANGE_MESSAGE);
    ASSERT_NULL(cmd->parameter);
    free_voice_command(cmd);
    
    printf("✓ Edge case parsing tests passed\n");
    return 0;
}

// Test command execution in demo mode
int test_execute_demo_mode(void) {
    printf("Testing command execution in demo mode...\n");
    
    // Test say hello command
    voice_command_t* cmd = parse_voice_command("say hello");
    ASSERT_NOT_NULL(cmd);
    int result = execute_voice_command(cmd, VOICE_SAFETY_DEMO);
    ASSERT_EQ(result, 0);  // Should succeed
    free_voice_command(cmd);
    
    // Test show code command
    cmd = parse_voice_command("show code");
    ASSERT_NOT_NULL(cmd);
    result = execute_voice_command(cmd, VOICE_SAFETY_DEMO);
    ASSERT_EQ(result, 0);  // Should succeed
    free_voice_command(cmd);
    
    // Test command with parameter
    cmd = parse_voice_command("change message Test");
    ASSERT_NOT_NULL(cmd);
    result = execute_voice_command(cmd, VOICE_SAFETY_DEMO);
    ASSERT_EQ(result, 0);  // Should succeed
    free_voice_command(cmd);
    
    // Test invalid command
    cmd = parse_voice_command("invalid command");
    ASSERT_NOT_NULL(cmd);
    result = execute_voice_command(cmd, VOICE_SAFETY_DEMO);
    ASSERT_EQ(result, 1);  // Should fail with exit code 1
    free_voice_command(cmd);
    
    printf("✓ Demo mode execution tests passed\n");
    return 0;
}

// Test memory management
int test_memory_management(void) {
    printf("Testing memory management...\n");
    
    // Test multiple allocations and frees
    for (int i = 0; i < 100; i++) {
        voice_command_t* cmd = parse_voice_command("change message Test message");
        ASSERT_NOT_NULL(cmd);
        ASSERT_NOT_NULL(cmd->parameter);
        free_voice_command(cmd);
    }
    
    // Test freeing NULL pointer (should not crash)
    free_voice_command(NULL);
    
    printf("✓ Memory management tests passed\n");
    return 0;
}

// Test command type string conversion
int test_command_type_strings(void) {
    printf("Testing command type string conversion...\n");
    
    ASSERT_STR_EQ(voice_command_type_string(VOICE_CMD_SAY_HELLO), "SAY_HELLO");
    ASSERT_STR_EQ(voice_command_type_string(VOICE_CMD_CHANGE_MESSAGE), "CHANGE_MESSAGE");
    ASSERT_STR_EQ(voice_command_type_string(VOICE_CMD_SHOW_CODE), "SHOW_CODE");
    ASSERT_STR_EQ(voice_command_type_string(VOICE_CMD_INSERT_CODE), "INSERT_CODE");
    ASSERT_STR_EQ(voice_command_type_string(VOICE_CMD_NAVIGATE_TO), "NAVIGATE_TO");
    ASSERT_STR_EQ(voice_command_type_string(VOICE_CMD_REFACTOR), "REFACTOR");
    ASSERT_STR_EQ(voice_command_type_string(VOICE_CMD_UNKNOWN), "UNKNOWN");
    
    printf("✓ Command type string tests passed\n");
    return 0;
}

int main(void) {
    printf("Running voice command parser unit tests...\n\n");
    
    int failed = 0;
    
    failed += test_parse_basic_commands();
    failed += test_parse_parameterized_commands();
    failed += test_parse_edge_cases();
    failed += test_execute_demo_mode();
    failed += test_memory_management();
    failed += test_command_type_strings();
    
    if (failed == 0) {
        printf("\n✓ All unit tests passed!\n");
        return 0;
    } else {
        printf("\n✗ %d test(s) failed\n", failed);
        return 1;
    }
}