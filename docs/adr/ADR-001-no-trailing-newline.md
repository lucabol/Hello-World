# ADR-001: No Trailing Newline in Output

## Status
Accepted

## Context
When designing a simple "Hello World" program in C, one common decision is whether the output should include a trailing newline character after the message. In many C programs, it's conventional to end printed lines with `\n` for proper terminal formatting. However, this project has specific requirements that led to a different choice.

The key considerations were:
1. **Exact output control**: The program needs to produce precise, predictable output that can be validated byte-for-byte
2. **Testing requirements**: Automated tests need to verify exact output matching
3. **Simplicity**: The program should demonstrate minimal C code without unnecessary formatting
4. **Cross-platform consistency**: Output should be identical across different platforms and terminals

## Decision
The program outputs `"Hello world!"` **without** a trailing newline character. This is achieved by using `printf("Hello world!");` instead of `printf("Hello world!\n");`.

## Consequences

### Positive
- **Precise testing**: Enables exact byte-level validation of output (see ADR-002)
- **Predictable behavior**: No platform-specific newline variations (LF vs CRLF)
- **Minimal code**: Demonstrates the simplest possible printf usage
- **Clear intent**: Makes it explicit that the program outputs exactly what is specified

### Negative
- **Terminal behavior**: On most terminals, the output will not move the cursor to the next line, potentially causing the shell prompt to appear on the same line as the output
- **Unconventional**: Most C programs end output with a newline for better terminal formatting
- **Documentation needed**: Requires explicit documentation to explain why this choice was made

### Neutral
- The lack of trailing newline is validated by test scripts using hex dump comparison to ensure compliance
- Any modification to add a newline would require updating all validation tests

## References
- Source code: `hello.c`
- Validation methodology: ADR-002-byte-level-validation.md
- Test implementation: `test/validate.sh`
