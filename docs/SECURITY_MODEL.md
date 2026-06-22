# SECURITY_MODEL

## Overview

Security in ASL defines how commands execute safely.

Focus: Privilege elevation and access control.

---

## Sudo Modifier

Syntax: `sudo <namespace> <verb> <object>`

The `sudo` flag elevates privileges for a single command.

It is evaluated first by the parser.

---

## Privilege Elevation Mechanism

The parser sets `sudo: true` in the Command Object.

The Tool Registry routes the command to the Tool.

The Tool is responsible for invoking the underlying Linux command using system `sudo` or equivalent polkit/pkexec mechanisms.

Example:
`sudo app install nano` -> Tool executes `sudo apt install nano`.

The system prompts the user for authentication natively.
ASL does NOT store or manage passwords directly.

---

## Future Scope

### Sandboxing

Tools should operate in isolated environments where applicable (e.g., containerized execution).

### Permission Isolation

Namespaces should define strict boundaries.
Example: A `browser` tool cannot execute `system` level commands.

### Validation

AI-generated commands (Phase 3) must be strictly validated against the Command Schema before execution.
Malformed or malicious commands must be rejected at the parser level.
