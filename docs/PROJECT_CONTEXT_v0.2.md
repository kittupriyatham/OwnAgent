# PROJECT_CONTEXT_v0.2

## Project
OwnAgent

Goal: Build an Agentic Shell / Agentic OS layer on Linux.

AI is optional and comes later.

Commands must work directly without requiring an LLM.

---

## Platform Decisions

Target OS:
- Linux

Development:
- X11/Xorg

Potential Production:
- Wayland

Primary Language:
- C++

---

## Core Architecture

User
↓
ASL Command
↓
Tokenizer
↓
Parser
↓
Command Object
↓
Tool Registry
↓
Tool
↓
Linux

Future AI Layer:

Natural Language
↓
LLM
↓
ASL
↓
Existing Pipeline

---

## Agentic Shell Language (ASL)

Syntax:

[sudo] <namespace> <verb> <object> [tool] [flags] [options]

Examples:

app open notepad
app close notepad
app install nano
app uninstall nano
browser open "google.com" chrome --headless
file open notes.txt nano
system shutdown

---

## Namespace Philosophy

IMPORTANT:

Namespaces are command domains.

Namespaces are NOT OS object types.

Example:

app open notepad

means:

Open application named notepad.

Example:

app install nano

means:

Install application/package nano.

Do not create a complex object hierarchy for v1.

---

## Preferred Namespaces

Use singular forms:

- app
- file
- browser
- window
- system
- network
- git
- docker
- service
- device
- agent

Avoid plural forms.

---

## Flags

Flags are boolean.

Examples:

--headless
--private
--recursive
--force

Representation:

{
  "flags": [
    "headless"
  ]
}

This is the chosen design.

Do NOT represent flags as:

{
  "headless": true
}

---

## Options

Examples:

--timeout 30
--port 5432
--branch dev

Representation:

{
  "options": {
    "timeout": 30,
    "port": 5432
  }
}

---

## Command Schema

{
  "sudo": false,
  "namespace": "browser",
  "verb": "open",
  "object": "google.com",
  "tool": "chrome",
  "flags": ["headless"],
  "options": {}
}

---

## Current Repository Documents

Existing:

- docs/ASL_v1.md
- docs/COMMAND_SCHEMA.json
- docs/ROADMAP.md
- docs/PROJECT_CONTEXT_v0.2.md

---

## Roadmap

Phase 1
- ASL
- Tokenizer
- Parser
- Command Schema
- Tool Registry

Phase 2
- File Tools
- App Tools
- Browser Tools
- System Tools

Phase 3
- Natural Language → ASL
- AI Translation Layer

Phase 4
- Desktop Control
- Window Management

Phase 5
- Memory System

Phase 6
- External Integrations

---

## Current Focus

Do NOT start:
- LLM integration
- GUI development
- Desktop environment development

Focus only on architecture and specifications.

---

## Next Documents To Create

1. docs/NAMESPACE_SPEC.md
2. docs/PARSER_SPEC.md
3. docs/TOOL_REGISTRY_SPEC.md
4. docs/SECURITY_MODEL.md

---

## Current Milestone

These commands should work before any AI exists:

app install nano
app open firefox
browser open google.com chrome --headless
file search "*.pdf"
system volume 50

Implement through:

Tokenizer
↓
Parser
↓
Tool Registry
↓
Tools

without any LLM.

---

## Instruction For Future Contributors

Treat this file as the authoritative project context.

Do not redesign the architecture.

Extend the specification incrementally.

Prioritize simplicity and direct command execution.
