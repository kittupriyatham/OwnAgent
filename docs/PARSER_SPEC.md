# PARSER_SPEC

## Overview

The Parser converts a token stream into a Command Object.

Target Language: C++

Input: Array of tokens from Tokenizer.

Output: Command Schema JSON/Struct.

---

## Behavior

The parser evaluates tokens sequentially.

Position dictates meaning for the first 3-4 arguments.

Syntax:
`[sudo] <namespace> <verb> <object> [tool] [flags] [options]`

1. Check for optional `sudo` token.
2. Require `namespace` token.
3. Require `verb` token.
4. Require `object` token.
5. Check for optional `tool` token before flags start.
6. Parse remaining tokens as `flags` (`--flag`) or `options` (`--key value`).

---

## Error Handling

The parser must fail gracefully and return explicit errors.

### Missing Verb

Condition: Token stream ends after namespace.

Action: Return `Error: Missing verb for namespace <namespace>`.

### Missing Object

Condition: Token stream ends after verb.

Action: Return `Error: Missing object for action <namespace> <verb>`.

### Unknown Tokens

Condition: Non-flag/option tokens found after the object/tool sequence.

Action: Return `Error: Unexpected token <token>`.

### Unmatched Quotes

Condition: Tokenizer passes incomplete string (e.g., `"google.com`).

Action: Tokenizer should catch this, but parser returns `Error: Malformed string token` if encountered.

### Invalid Flags/Options

Condition: Option key without value (e.g., `--timeout` at end of stream).

Action: Return `Error: Missing value for option <option>`.

---

## Happy Path Example

Input tokens:
`["browser", "open", "google.com", "chrome", "--headless"]`

Output:
```json
{
  "sudo": false,
  "namespace": "browser",
  "verb": "open",
  "object": "google.com",
  "tool": "chrome",
  "flags": ["headless"],
  "options": {}
}
```
