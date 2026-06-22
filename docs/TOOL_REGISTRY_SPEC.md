# TOOL_REGISTRY_SPEC

## Overview

The Tool Registry routes Command Objects to execution logic.

Target Language: C++

Input: Command Schema Struct.

Output: Tool Execution Result.

---

## Architecture

Implements the Registry Pattern.

Tools self-register during application startup.

Central registry maps `(namespace, verb, [tool])` to a C++ callback or Tool instance.

---

## Registration

Tools define what they handle.

Example C++ concept:
`Registry::register("app", "install", "nano", NanoInstallerTool);`
`Registry::register("browser", "open", "chrome", ChromeBrowserTool);`

A tool can register as the default handler.
`Registry::registerDefault("browser", "open", DefaultBrowserTool);`

---

## Routing Mechanism

1. Receive Command Object from Parser.
2. Extract `namespace`, `verb`, and `tool`.
3. Look up exact match in registry `(namespace, verb, tool)`.
4. If no exact match and no `tool` specified, fallback to default handler for `(namespace, verb)`.
5. If no handler found, return error: `Error: No tool registered for <namespace> <verb> <tool>` (omit `<tool>` if none was provided).
6. Invoke handler with Command Object (including `sudo`, `object`, `flags`, `options`).

---

## Execution Interface

All tools implement a common interface.

Must accept the parsed Command Object.

Must return a standard Result object (Success/Failure, Output String).

Tools are responsible for formatting the final Linux command.
