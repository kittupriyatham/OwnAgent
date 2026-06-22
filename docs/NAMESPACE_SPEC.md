# NAMESPACE_SPEC

## Overview

Namespaces define command domains in ASL.

Namespaces are NOT OS object types.

They group related actions logically.

---

## Naming Convention

Always use singular forms (per `docs/PROJECT_CONTEXT_v0.2.md`).

Avoid plural forms (e.g., prefer `app` over `apps`).

Note: `docs/ASL_v1.md` contains legacy plural examples; this spec supersedes that naming convention going forward.
Correct: `app`

Incorrect: `apps`

---

## Supported Namespaces

- `app`: Application and package management.
- `file`: File system operations.
- `browser`: Web browser and URL operations.
- `window`: Desktop window management.
- `system`: Core OS operations (volume, power).
- `network`: Connectivity and interface operations.
- `git`: Version control operations.
- `docker`: Container management.
- `service`: System daemon management.
- `device`: Hardware component operations.
- `agent`: OwnAgent core self-management.

---

## Philosophy

Namespaces trigger specific tool sets.

Example:

`app open notepad`

Meaning: "Use the application toolset to open the notepad process."

Example:

`app install nano`

Meaning: "Use the application toolset to install the nano package."

Do not create a complex object hierarchy for v1. Keep mappings direct.
