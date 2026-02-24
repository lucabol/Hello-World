# Routing

## Rules

| Pattern | Route To | Notes |
|---------|----------|-------|
| C code, implementation, refactoring, build issues | Neo | Primary developer |
| Architecture, scope, code review, design decisions | Morpheus | Lead reviews and decides |
| Tests, validation, edge cases, CI failures | Trinity | Quality assurance |
| Logging, decisions, session records | Scribe | Silent — never user-facing |
| Work queue, backlog, monitoring | Ralph | Work monitor |
| Multi-domain / "team" request | Morpheus + relevant agents | Lead coordinates |

## Fallback

When ambiguous, route to Neo (most tasks are implementation).
