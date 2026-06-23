# VALIDATOR_SPEC

## Overview

The Validator ensures that a parsed Command Object is legal before execution.

Target Language: C++

Input: Parsed `CommandModel`.

Output: `ValidationResult` structure.

---

## Behavior

The Validator asks the Registry "Is this legal?"

It does NOT use exceptions for validation logic.

It validates the following:
1. Command exists in Registry.
2. Action exists for Command.
3. Tool (if provided) is allowed for the Command/Action.
4. Target count matches the schema's `min_targets` and `max_targets`.
5. Flags provided are in `allowed_flags`.
6. Options provided are in `allowed_options`.

---

## Structured Validation Result

We use a standard C++ structure to represent success or failures.

```cpp
struct ValidationError {
    std::string code;
    std::string message;
};

struct ValidationResult {
    bool success;
    std::vector<ValidationError> errors;
};
```

---

## JSON Output Example

If a command fails validation, it generates JSON output directly matching the structures.

```json
{
    "success": false,
    "errors": [
        {
            "code": "UNKNOWN_TOOL",
            "message": "Tool 'chromee' is not registered for command 'browser'"
        },
        {
            "code": "INVALID_FLAG",
            "message": "Flag 'unknown' is not allowed for action 'open'"
        }
    ]
}
```

If successful:

```json
{
    "success": true,
    "errors": []
}
```
