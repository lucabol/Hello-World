# GitHub Actions — Morpheus Issue Routing

> **Note:** The following issues require label and comment updates. Execute these commands or use GitHub web UI to apply routing.

---

## IN-SCOPE: Route to Neo

### #1892: Add a function to print a custom message
```
Labels to add:
  - squad
  - squad:neo
  - type:enhancement
  - priority:medium

Comment:
---
✓ **APPROVED** — Morpheus (Lead)

This refactor maintains our output contract ("Hello world!", exit 0) and improves code structure via function encapsulation. 

**Assignment:** Neo — C Developer  
**Priority:** Medium  
**Notes:** Ensure `main()` still outputs exactly "Hello world!" to stdout.

---
```

### #1894: Use puts instead of printf
```
Labels to add:
  - squad
  - squad:neo
  - type:refactor
  - priority:low

Comment:
---
✓ **APPROVED (with clarification)** — Morpheus (Lead)

Simple substitution, but note: `puts()` appends a newline by default. 
To maintain output contract ("Hello world!" with no trailing newline), use `fputs()` instead:
  fputs("Hello world!", stdout);  // instead of printf or puts

**Assignment:** Neo — C Developer  
**Priority:** Low  
**Action:** Clarify implementation approach before proceeding.

---
```

---

## OUT-OF-SCOPE: Reject with Explanation

### #1890: Change the greeting to Italian
```
Labels to add:
  - type:out-of-scope
  - declined

Comment:
---
✗ **OUT OF SCOPE** — Morpheus (Lead)

This project has a strict output contract: produce exactly "Hello world!" with no trailing newline (ADR-001, CI validation via `make test`). 

Changing to "Ciao, Mondo!" would:
1. Break all existing CI/validation
2. Violate the project's specification
3. Require updating CI tests, which is beyond the scope of hello.c itself

**Recommendation:** If you want Italian output, create a separate branch or project.

---
```

### #1891: Add a newline after the greeting
```
Labels to add:
  - type:out-of-scope
  - declined

Comment:
---
✗ **OUT OF SCOPE** — Morpheus (Lead)

This directly violates **ADR-001: No Trailing Newline in Output**. The project requirement is:
- Output: "Hello world!" (no `\n`)
- Exit code: 0
- Validation: Byte-level verification in `test/validate.sh`

Adding a trailing newline breaks CI and the core specification.

**Recommendation:** Closed. This is a non-negotiable requirement for the Hello-World project.

---
```

### #1893: Print the program's exit code
```
Labels to add:
  - type:out-of-scope
  - declined

Comment:
---
✗ **OUT OF SCOPE** — Morpheus (Lead)

Printing the exit code would:
1. Change stdout output (violates output contract)
2. Create a chicken-and-egg problem (printing exit code before exiting)
3. Break CI validation

The hello.c project outputs exactly "Hello world!" to stdout and exits with code 0. Printing the exit code corrupts the output stream.

**Recommendation:** Closed. Use `echo $?` after running `./hello` to inspect the exit code externally.

---
```

### #1895: Integrate a visual block-based editor
```
Labels to add:
  - type:out-of-scope
  - epic:infrastructure-theater
  - declined

Comment:
---
✗ **OUT OF SCOPE** — Morpheus (Lead)

This project is a **minimal C "Hello World" program** (~5 lines of code). A web-based visual block editor would require:
- Backend infrastructure (Node/Python/Go server)
- Frontend framework (React, Vue, or Svelte)
- Build pipeline for web assets
- Drag-and-drop UI library
- Code generation / export logic

This is 100x the scope of the core project. We maintain simplicity as a core value.

**Recommendation:** Closed. If interested in visual C editors, explore existing projects like Scratch or Blockly; don't graft them onto Hello-World.

---
```

### #1896: Implement a spreadsheet-like interface for code metrics
```
Labels to add:
  - type:out-of-scope
  - epic:infrastructure-theater
  - declined

Comment:
---
✗ **OUT OF SCOPE** — Morpheus (Lead)

Building a spreadsheet tool for code metrics on a 5-line C program is:
1. **Massive over-engineering** — the code is too simple to have meaningful metrics
2. **Scope creep** — requires web UI, data visualization, metrics engine
3. **Disconnected** — unrelated to the core project goal

**Recommendation:** Closed. If you need code metrics, use standard tools like `cloc`, `cppcheck`, or `clang-tools`.

---
```

### #1897: Enable real-time collaborative editing
```
Labels to add:
  - type:out-of-scope
  - epic:infrastructure-theater
  - declined

Comment:
---
✗ **OUT OF SCOPE** — Morpheus (Lead)

Real-time collaborative editing requires:
- WebSocket infrastructure (Node/Python/Go)
- Operational Transformation (OT) or CRDT algorithm
- Conflict resolution logic
- Client-side live sync implementation
- Deployment/hosting for multi-user server

This is a **100+ engineer-hour feature** for a 5-line C file. Hello-World is not an IDE platform.

**Recommendation:** Closed. Use GitHub's native PR/code review for collaboration.

---
```

### #1898: Add support for voice-driven code editing
```
Labels to add:
  - type:out-of-scope
  - epic:infrastructure-theater
  - declined

Comment:
---
✗ **OUT OF SCOPE** — Morpheus (Lead)

Voice-driven code editing requires:
- Speech recognition API (Google Cloud Speech, Azure, Whisper)
- Natural language processing (NLP) for command interpretation
- Code generation from voice intent
- Fallback error handling for misrecognitions

This is a **cutting-edge AI feature** that dwarfs the scope of hello.c by orders of magnitude.

**Recommendation:** Closed. Explore voice AI in appropriate projects (IDE plugins, code assistants).

---
```

### #1899: Create a plugin system for extending functionality
```
Labels to add:
  - type:out-of-scope
  - epic:infrastructure-theater
  - declined

Comment:
---
✗ **OUT OF SCOPE** — Morpheus (Lead)

A plugin system for a 5-line program violates the **simplicity principle**. You'd need:
- Plugin loader/unloader
- Symbol resolution / dynamic linking
- Version compatibility management
- Plugin discovery/registry
- Security/sandboxing

This turns a minimal Hello World into a **plugin platform**, fundamentally changing the project's purpose.

**Recommendation:** Closed. The project is intentionally simple. Don't over-architect.

---
```

---

## Summary

- **Approved:** 2 issues (#1892, #1894) → Route to Neo
- **Declined:** 8 issues (#1890–#1891, #1893–#1899) → Close with scope explanation
- **Pattern:** Issues #1895–#1899 exemplify "infrastructure theater" — ambitious but misaligned with project scope.
- **Decision:** Morpheus enforces scope boundaries. Simplicity is the project's competitive advantage.
