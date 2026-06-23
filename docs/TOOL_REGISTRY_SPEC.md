# TOOL_REGISTRY_SPEC

## Overview

The Tool Registry manages what commands, actions, and tools exist in the system.

Target Language: C++

Input: Registry queries.

Output: Schemas and availability boolean checks.

---

## Terminology Updates

We use the canonical model terminology.

Do NOT use: namespace, verb, object.

USE: `command`, `action`, `target`.

---

## Architecture

Implements a static registry for Milestone 2.

Registries are hardcoded in C++ source files (not loaded from JSON yet).

The Registry provides a defined Schema for every `command.action` pair.

---

## Registration & Schemas

The registry declares a schema for each supported action.

Example conceptual schema for `browser.open`:
- `min_targets`: 0
- `max_targets`: -1 (unlimited)
- `allowed_tools`: ["chrome", "firefox", "edge", "brave"]
- `allowed_flags`: ["headless", "private"]
- `allowed_options`: ["timeout", "profile"]

Example conceptual schema for `app.install`:
- `min_targets`: 1
- `max_targets`: -1 (unlimited)
- `allowed_tools`: []
- `allowed_flags`: ["force"]
- `allowed_options`: []

---

## Registry Responsibilities

The Registry is used by the Validator to ask:
"What exists?"

It answers questions like:
- Does command `app` exist?
- Does action `open` exist for `app`?
- What are the allowed tools for `browser` `open`?
- What is the target boundary for `file` `open`?

---

## Execution Interface (Future)

Currently, the Tool Registry only manages schema and validation paths.

Execution routing will be implemented in future phases.
