# Issue Triage — Morpheus

**Date:** 2026-02-24  
**Batch:** Issues #1890–#1899 (10 items)  
**Triager:** Morpheus (Lead)

---

## Triage Decisions

### IN-SCOPE Issues (2/10)

| # | Title | Route | Priority | Notes |
|---|-------|-------|----------|-------|
| #1892 | Add a function to print custom message | Neo (squad:neo) | medium | Reasonable refactor; maintains output contract. Function encapsulation is good architecture. |
| #1894 | Use puts instead of printf | Neo (squad:neo) | low | Simple substitution; `puts()` adds newline, so must use `fputs()` or adjust spec. Clarify output impact. |

### OUT-OF-SCOPE Issues (8/10)

| # | Title | Reason | Notes |
|---|-------|--------|-------|
| #1890 | Italian greeting | CI validation conflict | Output contract is "Hello world!" Changing to "Ciao, Mondo!" breaks CI. Scope boundary. |
| #1891 | Add newline after greeting | ADR-001 violation | No trailing newline is a hard requirement (ADR-001, byte-level validation). Rejects this. |
| #1893 | Print exit code | Behavior contract violation | Would corrupt output stream and violate exit code contract. Out of scope. |
| #1895 | Visual block-based editor | Extreme scope creep | Web UI for visual C editing. Requires backend, React/Vue, entirely separate from 5-line C program. |
| #1896 | Spreadsheet code metrics | Extreme scope creep | Spreadsheet tool for analyzing code metrics. Unrelated tooling. |
| #1897 | Real-time collaborative editing | Extreme scope creep | Multi-user real-time sync requires WebSocket infrastructure, conflict resolution. Orders of magnitude beyond scope. |
| #1898 | Voice-driven code editing | Extreme scope creep | Voice command interface. Requires speech recognition. Wildly out of scope. |
| #1899 | Plugin system | Extreme scope creep | Plugin architecture for a 5-line C program. Over-engineered; violates simplicity principle. |

---

## Scope Boundaries

**In-scope = hello.c modifications that:**
- Maintain exact output contract: "Hello world!" + exit 0
- Stay within C standard library (stdio.h)
- Keep codebase under ~50 lines
- Pass CI validation (make test)

**Out-of-scope = anything that:**
- Changes output or exit code
- Adds external dependencies/frameworks
- Introduces web UI, cloud infrastructure, or plugins
- Grows code >10x
- Violates ADRs

---

## Key Pattern

Issues #1895–#1899 represent **infrastructure theater** — ambitious but disconnected from project purpose. This is a **minimal C Hello World**, not a code platform. Recommend closing these with clear scope explanation.

---

## Next Steps

1. Neo picks up #1892 and #1894 (flagged with squad:neo)
2. Close #1890, #1891, #1893 with scope explanation comments
3. Close #1895–#1899 with clear scope rejection
