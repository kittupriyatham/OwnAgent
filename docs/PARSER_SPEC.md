# PARSER_SPEC

## Overview

The Parser converts a token stream into a Command Target.

Target Language: C++

Input: Array of tokens from Tokenizer.

Output: Command Schema JSON/Struct.

---

## Behavior

The parser evaluates tokens sequentially.

Position dictates meaning for the first 3-4 arguments.

Syntax:
`[sudo] <command> <action> [target] [tool] [flags] [options]`
1. Check for optional `sudo` token.
2. Require `command` token.
3. Require `action` token.
4. If present, parse the next non-flag token as `target`.
5. Check for optional `tool` token before flags start.
6. Parse remaining tokens as `flags` (`--flag`) or `options` (`--key value`).

---

## Error Handling

The parser must fail gracefully and return explicit errors.

### Missing Action

Condition: Token stream ends after command.

Action: Return `Error: Missing action for command <command>`.

### Missing Target (when required)

Condition: Token stream ends after action for a action/tool that requires an target.

Action: Return `Error: Missing target for action <command> <action>`.
### Unknown Tokens

Condition: Non-flag/option tokens found after the target/tool sequence.

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
  "command": "browser",
  "action": "open",
  "target": ["google.com"],
  "tool": "chrome",
  "flags": ["headless"],
  "options": {}
}
```
