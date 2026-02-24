# Morpheus — History

## Project Context
- **Project:** Hello-World — minimal C "Hello World" program
- **Stack:** C, GCC/Clang, Make
- **User:** Luca Bolognese
- **Key files:** hello.c (main source), Makefile, test/validate.sh

## Learnings

### Issue Triage — #1890–#1899 (2026-02-24)
- **Pattern:** Heavy scope creep in issues #1895–#1899 (visual editors, spreadsheets, voice UIs, plugins). These are **infrastructure theater** unfit for a minimal C project.
- **Scope Boundary:** Accept only hello.c changes that maintain exact output "Hello world!" and exit 0. Reject anything that changes output, adds external deps, or grows code >10x.
- **ADR Enforcement:** #1891 violates ADR-001 (no trailing newline). #1890 violates output contract. Both rejected.
- **In-Scope:** #1892 (custom message function), #1894 (printf→puts, needs output clarification).
- **Decision:** Close #1890–#1891, #1893–#1899 with clear scope explanation. Route #1892, #1894 to Neo.
