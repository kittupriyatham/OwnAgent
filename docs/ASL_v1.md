# Agentic Shell Language (ASL) v1

## Command Format

```text
[sudo] <namespace> <verb> <object> [tool] [flags] [options]
```

Examples:

```text
apps install nano
sudo apps install nano
browser open "google.com" chrome --headless
files open notes.txt nano
```

## Components

### Namespace

Reserved namespaces:

- files
- apps
- windows
- system
- network
- browser
- git
- docker
- user
- service
- device
- agent

### Verb

Action performed by a namespace.

Examples:

- open
- close
- install
- uninstall
- search
- update

### Object

Primary target of the command.

Examples:

- nano
- firefox
- notes.txt
- google.com

### Tool

Optional executor.

Examples:

- nano
- vim
- chrome
- firefox

### Flags

Boolean modifiers.

Examples:

```text
--headless
--recursive
--force
--private
```

JSON:

```json
{
  "flags": ["headless", "private"]
}
```

### Options

Key-value modifiers.

Examples:

```text
--timeout 30
--port 5432
--branch dev
```

JSON:

```json
{
  "options": {
    "timeout": 30,
    "port": 5432
  }
}
```
