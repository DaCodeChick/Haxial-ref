# Naming Policy Clarification - Final Version

## The Problem with Placeholder Names

**What I was doing wrong:**
- Renaming `param_1` → `temp1`
- Renaming `uVar2` → `flags`
- Renaming `local_8c` → `result`
- Creating struct fields like `field_0x08`

**Why this is wrong:**
- `temp1` hides that you don't know what it is (looks like a name, but means nothing)
- `flags` pretends you understand it's flags (false confidence)
- `result` is vague and uninformative (what kind of result?)
- `field_0x08` creates fake field names instead of noting offset

## The Correct Approach

### For Variables

**Unknown variables: LEAVE AS-IS**

```c
// DON'T DO THIS:
void function(int socket, int temp1, int flags) {
    int result = temp1 + flags;
}

// DO THIS:
void function(int param_1, int param_2, int param_3) {
    int uVar1 = param_2 + param_3;
}
// Then document: "param_2 - offset calculation? param_3 - some kind of flags?"
```

**Known variables: USE SPECIFIC NAMES**

```c
// DO THIS:
void socket_shutdown(SocketInfo *socket) {
    if (socket->magic == 0x42781ea5) {
        shutdown(socket->socket_fd, 2);
    }
}
```

**Partially known variables: USE DESCRIPTIVE NAME + [GUESS]**

```c
// DO THIS:
struct SocketInfo {
    uint magic;                    // +0x00 - KNOWN
    uint lock_or_refcount;         // +0x04 - GUESS: could be either
    uint socket_fd;                // +0x10 - KNOWN
};
```

### For Struct Fields

**Don't create field names for unknowns - note the offsets in documentation**

```c
// DON'T DO THIS:
struct SocketInfo {
    uint magic;           // +0x00
    uint field_0x04;      // +0x04 - fake field name
    uint field_0x08;      // +0x08 - fake field name
    uint socket_fd;       // +0x10
};

// DO THIS:
struct SocketInfo {
    uint magic;           // +0x00 - KNOWN
    uint lock_or_refcount; // +0x04 - GUESS
    byte gap_0x08[8];     // +0x08 - UNKNOWN: document accesses
    uint socket_fd;       // +0x10 - KNOWN
};
```

**Then in unknowns-to-revisit.md:**
```markdown
### SocketInfo Structure
- **gap_0x08[8]** - Unknown 8-byte region
  - Accesses seen: param_1[2] in socket_send_or_queue
  - Offset calculation: param_1[2] = +0x08
  - Candidates: event_context pointer, flags, padding
```

### For Function Calls

**Leave FUN_xxxxxxxx as-is until you analyze them**

```c
// DON'T DO THIS:
void socket_send_or_queue(...) {
    queue_entry = allocate_queue_entry();  // Fake name, didn't analyze it
    add_to_queue(queue_entry);              // Fake name, didn't analyze it
}

// DO THIS:
void socket_send_or_queue(...) {
    queue_entry = FUN_08098d3c();
    FUN_0806edd8(param_1 + 9, queue_entry);
}
// Then document: "FUN_08098d3c - allocates something, need to analyze"
```

## Documentation Examples

### Example 1: Unknown Variable

```markdown
### socket_send_or_queue (0x08098104)
- **param_1[9]** (offset +0x24) - What is this pointer?
  - Used: Passed to FUN_0806edd8, checked against 0
  - Question: Is this a queue head? List head? Something else?
  - Related: FUN_08098d3c seems to allocate entries for this
```

### Example 2: Unknown Struct Offset

```markdown
### SocketInfo Accesses
- **+0x08** - Accessed as param_1[2] in multiple functions
  - Functions: socket_send_or_queue, create_socket
  - Usage: Passed to FUN_0808fcfc
  - Candidates: event_context? file descriptor? handle?
```

### Example 3: Partially Known (GUESS)

```markdown
### SocketInfo Field +0x04
- **lock_or_refcount** - Could be either
  - Used: Incremented in some functions, passed to lock functions in others
  - Evidence for lock: lock_acquire/release call patterns
  - Evidence for refcount: Incremented on creation, decremented on close
  - TODO: Trace usage in all socket functions to determine which
```

## The Three-Tier System (Simplified)

1. **KNOWN** → Use specific name
   - Example: `socket_fd`, `bytes_sent`, `error_code`

2. **GUESS** → Use descriptive name + [GUESS] marker
   - Example: `lock_or_refcount [GUESS]`, `event_context [GUESS]`

3. **UNKNOWN** → Leave as-is, document offset/usage
   - Example: Keep `param_1`, `uVar2`, note "param_1[9] offset +0x24"

## Key Principle

**Visibility of ignorance is a feature, not a bug**

- `param_1` clearly says "I don't know what this is yet"
- `temp1` pretends to be a name but is meaningless
- `uVar2` is obviously placeholder from Ghidra
- `flags` falsely implies understanding

## Updated Policy

### DO:
- ✓ Leave param_1, uVar2, local_8c when you don't know
- ✓ Document offset accesses: "param_1[9] = offset +0x24"
- ✓ Use gap_0xNN[size] for unknown struct regions
- ✓ Mark guesses explicitly: [GUESS: ...]
- ✓ Note all accesses in unknowns documentation

### DON'T:
- ✗ Rename to temp1, temp2, temp3
- ✗ Rename to generic words: flags, result, data, ptr
- ✗ Create fake struct field names: field_0x08
- ✗ Pretend to know things by using specific-sounding names
- ✗ Invent names for numbered variants: flags1, flags2

## Summary

**Old (Wrong) Approach:**
- param_1 → temp1 (hides ignorance)
- uVar2 → flags (false confidence)
- Create field_0x08 (fake structure)

**New (Correct) Approach:**
- param_1 stays param_1 (obvious unknown)
- Document "param_1[9] offset +0x24, usage unclear"
- Use gap_0x08[8] with documented accesses

This approach makes it crystal clear what's known vs unknown, without creating the illusion of understanding through placeholder names.

