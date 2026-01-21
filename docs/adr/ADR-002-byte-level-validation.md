# ADR-002: Byte-Level Validation

## Status
Accepted

## Context
Testing a simple program like "Hello World" requires a validation strategy. Different approaches exist:
1. **Simple string comparison**: Compare program output as strings
2. **Regex matching**: Use pattern matching for flexible validation
3. **Byte-level verification**: Compare output at the byte level using hex dumps
4. **Exit code checking**: Verify the program exits successfully

For this project, the validation needs to ensure:
- Exact output compliance (see ADR-001)
- No hidden characters or formatting differences
- Consistent behavior across different environments and compilers
- Detection of any unintended modifications to output

## Decision
The project uses **byte-level validation** through hex dump comparison combined with exit code verification. The validation process includes:

1. **Compilation check**: Build with strict compiler flags (-Werror) to ensure no warnings or errors
2. **Output capture**: Run the program and capture its output
3. **Hex dump comparison**: Compare the actual output with expected output at the byte level
4. **Exit code verification**: Ensure the program returns exit code 0
5. **Cross-compiler validation**: Verify identical output from both GCC and Clang (see ADR-003)

The validation script (`test/validate.sh`) implements this methodology:
```bash
# Expected output in hex (48 65 6c 6c 6f 20 77 6f 72 6c 64 21 = "Hello world!")
actual_hex=$(./hello | od -An -tx1 | tr -d ' \n')
expected_hex="48656c6c6f20776f726c6421"

if [ "$actual_hex" = "$expected_hex" ]; then
    # Validation passes
fi
```

## Consequences

### Positive
- **Unambiguous verification**: Detects any byte-level differences, including hidden characters
- **No false positives**: String encoding or line ending issues are caught immediately
- **Reproducible**: Same validation works identically across all platforms
- **Comprehensive**: Validates both content and absence of trailing newline
- **CI/CD friendly**: Easy to integrate into automated workflows

### Negative
- **More complex**: Requires understanding of hex dumps and byte-level comparison
- **Less readable**: The test code is less intuitive than simple string comparison
- **Debugging**: When validation fails, requires hex dump analysis to understand the difference

### Neutral
- This validation approach is well-suited for programs with exact, fixed output requirements
- For programs with variable output, a different validation strategy might be more appropriate
- The methodology serves as documentation of the exact expected behavior

## References
- Output specification: ADR-001-no-trailing-newline.md
- Test implementation: `test/validate.sh`
- CI workflow: `.github/workflows/ci.yml`
