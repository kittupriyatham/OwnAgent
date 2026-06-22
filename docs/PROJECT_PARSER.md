# PROJECT_PARSER

> Terminology note: `command` == `namespace`, `action` == `verb`, and `target` is the list-form of `object` (to support multiple targets).

## Canonical Command Model

```json
{
    "sudo": false,
    "command": "app",
    "action": "open",
    "tool": null,
    "target": ["chrome"],
    "flags": [],
    "options": {}
}
```

---

## Field Definitions

### sudo

Boolean.

Example:

```json
{
  "sudo": true
}
```

---

### command

Subsystem/domain being addressed.

Examples:

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

---

### action

Operation to perform.

Examples:

- open
- close
- install
- uninstall
- update
- delete
- copy
- move
- search

---

### tool

Optional implementation/tool/application.

Examples:

```json
{
  "tool": "chrome"
}
```

```json
{
  "tool": "firefox"
}
```

Can be null.

---

### target

Always a list.

Even a single target is represented as a list.

Single target:

```json
{
  "target": ["google.com"]
}
```

Multiple targets:

```json
{
  "target": [
    "google.com",
    "github.com"
  ]
}
```

---

### flags

Boolean modifiers.

Example:

```json
{
  "flags": [
    "headless",
    "private"
  ]
}
```

---

### options

Key-value arguments.

Example:

```json
{
  "options": {
    "timeout": 30,
    "port": 8080
  }
}
```

---

## Examples

### Open Chrome Application

Command:

```text
app open chrome
```

AST:

```json
{
  "command": "app",
  "action": "open",
  "tool": null,
  "target": ["chrome"]
}
```

---

### Open Default Browser Application

Command:

```text
app open browser
```

Meaning:

- Open default browser application.
- If not running, launch browser.
- If already running, open a new browser window.

---

### Open Browser Window

Command:

```text
browser open
```

Meaning:

- Browser action.
- If no browser instance exists, launch default browser.
- Restore previous session according to browser settings.
- Open a browser window.
- If browser already running, open a new browser window.

AST:

```json
{
  "command": "browser",
  "action": "open",
  "tool": null,
  "target": []
}
```

---

### Open URL In Default Browser

Command:

```text
browser open google.com
```

Meaning:

- Open a new tab.
- Navigate to google.com.

AST:

```json
{
  "command": "browser",
  "action": "open",
  "tool": null,
  "target": ["google.com"]
}
```

---

### Open URL In Chrome

Command:

```text
browser open google.com chrome
```

AST:

```json
{
  "command": "browser",
  "action": "open",
  "tool": "chrome",
  "target": ["google.com"]
}
```

---

### Open Multiple URLs In Chrome

Command:

```text
browser open google.com github.com chrome
```

AST:

```json
{
  "command": "browser",
  "action": "open",
  "tool": "chrome",
  "target": [
    "google.com",
    "github.com"
  ]
}
```

---

## Parser Goal (Milestone 1)

Input:

```text
browser open google.com chrome --headless --timeout 30
```

Output:

```json
{
  "sudo": false,
  "command": "browser",
  "action": "open",
  "tool": "chrome",
  "target": ["google.com"],
  "flags": ["headless"],
  "options": {
    "timeout": 30
  }
}
```

No tool execution.
No AI.
No Linux integration.
Only parsing, validation and serialization.
