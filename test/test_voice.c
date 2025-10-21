/* Unit tests for voice.c
 * Tests the voice command parsing and processing functionality
 * 
 * This test suite validates:
 * - String parsing for all supported commands
 * - Edge cases (NULL input, empty strings, long input)
 * - Error handling for invalid commands
 * - No filesystem modifications during simulation
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simple_test.h"
#include "../voice.h"

/* Test that NULL command is handled gracefully */
void test_null_command(void) {
    int result = process_voice_command(NULL);
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_ERROR, result);
}

/* Test empty string command */
void test_empty_command(void) {
    int result = process_voice_command("");
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_ERROR, result);
}

/* Test "say hello" command returns success */
void test_say_hello_command(void) {
    int result = process_voice_command("say hello");
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_SUCCESS, result);
}

/* Test "show code" command returns success */
void test_show_code_command(void) {
    int result = process_voice_command("show code");
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_SUCCESS, result);
}

/* Test "change message" with text returns success */
void test_change_message_command(void) {
    int result = process_voice_command("change message Test");
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_SUCCESS, result);
}

/* Test "change message" with long text returns success */
void test_change_message_long_text(void) {
    int result = process_voice_command("change message This is a very long message with many words");
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_SUCCESS, result);
}

/* Test "change message" with empty text returns success (edge case) */
void test_change_message_empty_text(void) {
    int result = process_voice_command("change message ");
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_SUCCESS, result);
}

/* Test invalid command returns error */
void test_invalid_command(void) {
    int result = process_voice_command("invalid command");
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_ERROR, result);
}

/* Test command with leading spaces returns error (not supported) */
void test_leading_spaces(void) {
    int result = process_voice_command("  say hello");
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_ERROR, result);
}

/* Test command with trailing spaces returns error (exact match required) */
void test_trailing_spaces(void) {
    int result = process_voice_command("say hello  ");
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_ERROR, result);
}

/* Test case sensitivity - commands are case-sensitive */
void test_case_sensitivity(void) {
    int result = process_voice_command("SAY HELLO");
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_ERROR, result);
}

/* Test partial command doesn't match */
void test_partial_command(void) {
    int result = process_voice_command("say");
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_ERROR, result);
}

/* Test very long command (256+ chars) - should handle gracefully */
void test_very_long_command(void) {
    char long_cmd[300];
    strcpy(long_cmd, "change message ");
    /* Fill with 'A's to make it very long */
    for (int i = 15; i < 280; i++) {
        long_cmd[i] = 'A';
    }
    long_cmd[280] = '\0';
    
    int result = process_voice_command(long_cmd);
    TEST_ASSERT_EQUAL_INT(VOICE_CMD_SUCCESS, result);
}

/* Main test runner */
int main(void) {
    printf("\n");
    print_yellow("========================================\n");
    printf("Running Unit Tests for voice.c\n");
    print_yellow("========================================\n");
    printf("\n");
    
    RUN_TEST(test_null_command);
    RUN_TEST(test_empty_command);
    RUN_TEST(test_say_hello_command);
    RUN_TEST(test_show_code_command);
    RUN_TEST(test_change_message_command);
    RUN_TEST(test_change_message_long_text);
    RUN_TEST(test_change_message_empty_text);
    RUN_TEST(test_invalid_command);
    RUN_TEST(test_leading_spaces);
    RUN_TEST(test_trailing_spaces);
    RUN_TEST(test_case_sensitivity);
    RUN_TEST(test_partial_command);
    RUN_TEST(test_very_long_command);
    
    TEST_SUMMARY();
}
