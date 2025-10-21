# Security Policy

## Input Sanitization

The Visual Block Editor implements comprehensive input sanitization to ensure generated C code is safe and syntactically correct:

### Include Headers
- **Whitelist validation**: Only allows predefined safe headers (stdio.h, stdlib.h, string.h, math.h)
- Any attempt to use non-whitelisted headers defaults to stdio.h
- No arbitrary header names are permitted

### Printf Text Strings
- **Automatic escaping**: All special C string characters are properly escaped:
  - Backslashes (`\`) → `\\`
  - Double quotes (`"`) → `\"`
  - Newlines (`\n`) → `\\n`
  - Carriage returns (`\r`) → `\\r`
  - Tabs (`\t`) → `\\t`
- Prevents code injection through string literals
- All user input is treated as plain text and never evaluated as code

### Return Values
- **Numeric validation**: Only integer values are accepted
- Non-numeric input is stripped
- Defaults to 0 if invalid input is provided
- Prevents injection of arbitrary C expressions

### Code Generation
- No use of `eval()` or dynamic code execution
- No innerHTML with unsanitized user content
- All user inputs are validated before rendering in the DOM
- Generated code is constructed through template literals with sanitized values only

## Export Behavior

- The editor **does not** automatically modify files in your repository
- Exported code is downloaded to your browser's default download location
- Users must manually replace repository files with downloaded code
- The editor cannot and does not access your file system directly

## Security Limitations

### User Responsibility
- Once code is exported, users can edit the .c file with any content
- The editor only controls what it generates, not what users do with exported files
- Always review exported code before compilation

### Browser Security
- The editor runs entirely in the browser with no server-side component
- Subject to standard browser security policies (same-origin, CSP, etc.)
- No data is transmitted to external servers

## Reporting Security Issues

If you discover a security vulnerability in the visual editor, please report it by:
1. Creating a private security advisory on GitHub
2. Or emailing the repository maintainers directly

Do not create public issues for security vulnerabilities.

## Best Practices

When using the visual editor:
1. Review generated code before compiling
2. Compile with warnings enabled (`gcc -Wall -Wextra`)
3. Test in a safe environment first
4. Keep your browser up to date for latest security patches
