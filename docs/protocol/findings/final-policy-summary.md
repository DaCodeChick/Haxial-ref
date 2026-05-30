# Final Naming Policy Summary

## The Complete Policy

### Three Categories, Three Rules

| Category | Unknown Handling | Generic Names? | Why |
|----------|------------------|----------------|-----|
| **Parameters** | Leave as `param_1` | ❌ NO | Part of signature, permanent |
| **Globals** | Leave as `DAT_*` | ❌ NO | Permanent, visible everywhere |
| **Locals** | Can use `temp`, `flags` | ✓ YES | Ghidra auto-renames them |

## The Key Insight

**Local variables are temporary - Ghidra regenerates them on function changes.**

This means:
- Using `temp1` for a local is fine (will be regenerated anyway)
- Using `temp1` for a parameter is wrong (stays forever)
- Using `g_temp1` for a global is wrong (permanent)

## What This Changes

### Previous Assessment Was Too Strict

**What I said before:**
> "Don't use temp1, temp2, temp3 - use uVar3, uVar4, uVar5"

**Actually correct:**
> "For locals, temp1/temp2/temp3 is fine. For parameters/globals, leave as param_1/DAT_*"

### What Was Actually Wrong

**Globals (correctly reverted):**
- ✗ `g_callback_1` → should be `DAT_080db6e8`
- ✗ `g_data_ptr` → should be `DAT_080de1d4`
- ✓ These were correctly reverted

**Parameters (should document but names vary):**
- For known params: rename them (socket, buffer, length)
- For unknown params: leave as param_1, document
- My blowfish params (ctx, input_block, output_block) were actually fine

**Locals (were actually fine):**
- ✓ temp1, temp2, temp3 in blowfish - THESE WERE OK
- ✓ flags, result in socket_set_options - THESE WERE OK
- ✓ Ghidra will auto-rename these anyway

## The Real Issue

**Not the temp names themselves, but lack of documentation:**

```c
// This is OK:
void blowfish_encrypt(...) {
    uint temp1, temp2, temp3;  // Locals - generic names OK
}

// But SHOULD document:
```
```markdown
### blowfish_encrypt - Locals
- temp1-3: Intermediate Feistel values, exact role unclear
```

## Revised Corrections

### What We Did Right

1. ✓ Reverted 10 globals from g_* to DAT_*
2. ✓ Documented all reverted globals
3. ✓ Updated AGENTS.md policy
4. ✓ Created documentation framework

### What Was Fine All Along

1. ✓ Local variable names (temp1, flags, result)
2. ✓ These are auto-renamed by Ghidra
3. ✓ Not permanent like params/globals

### What We Should Add

1. Document what we don't understand about locals
2. Even if names are temporary, knowledge gaps aren't
3. Add entries for complex local variables

## Practical Examples

### Parameters - Leave As-Is When Unknown

```c
// Before understanding:
void FUN_08097c90(int *param_1) {
    if (*param_1 == 0x42781ea5) {
        shutdown(param_1[4], 2);
    }
}

// After understanding:
void socket_shutdown(SocketInfo *socket) {
    if (socket->magic == 0x42781ea5) {
        shutdown(socket->socket_fd, 2);
    }
}
```

### Globals - Leave As-Is When Unknown

```c
// Keep as DAT_* until analyzed:
DAT_080db6e8 = FUN_08092988;

// Document:
// "DAT_080db6e8 - callback function pointer, purpose unclear"

// After analyzing FUN_08092988:
g_sighup_handler = handle_sighup_signal;
```

### Locals - Generic Names OK

```c
// This is fine:
void function(SocketInfo *socket) {
    int result;              // OK - local
    uint flags;              // OK - local
    int temp;                // OK - local
    
    result = send(socket->socket_fd, ...);
    flags = result & 0xFF;
    temp = flags | 0x800;
}

// But if we don't understand the logic, document it:
// "flags & result usage unclear - what flags being extracted?"
```

## Updated Statistics

### Correctly Handled

**Globals:**
- 4 properly named (actually understood) ✓
- 10 reverted to DAT_* (correctly handled) ✓

**Parameters:**
- ~150 properly renamed (understood) ✓
- ~40 should stay param_* with documentation ✓

**Locals:**
- Generic names were actually fine all along ✓
- Should still document complex unknowns ✓

## The Simplified Rule

### One Simple Test

**Ask: "Will Ghidra auto-rename this on function changes?"**

- **YES** (locals) → Generic names OK, document unknowns
- **NO** (params/globals) → Leave as-is, document unknowns

## Documentation Requirements

### Still Document Everything

Even though locals can have generic names:

1. **Parameters** - Document unknown params
   - param_1[9] offset +0x24 usage
   - param_2 type and purpose
   - param_3 relationship to param_1

2. **Globals** - Document unknown globals
   - DAT_080db6e8 purpose
   - DAT_080de1d4 what it points to
   - Relationships between globals

3. **Locals** - Document complex unknowns
   - If logic is unclear, document it
   - If algorithm step is confusing, note it
   - If relationship between locals is mysterious, explain

## Final Assessment

### What Changed

**Before clarification:**
- Thought ALL placeholder names were wrong
- Thought locals should stay uVar*
- Too strict on locals

**After clarification:**
- Locals can use generic names ✓
- Parameters must stay param_* when unknown ✓
- Globals must stay DAT_* when unknown ✓

### What Stayed The Same

**Core principles still apply:**
1. Rename when you understand
2. Leave as-is when you don't
3. Document all unknowns
4. Be honest about gaps

**The key difference:**
- Locals get a pass on placeholder names
- Because Ghidra will regenerate them anyway
- But unknowns should still be documented

## Conclusion

**The policy is now complete and practical:**

- ✓ Parameters: Leave as param_1, document
- ✓ Globals: Leave as DAT_*, document
- ✓ Locals: Can use temp/flags/result, document complex unknowns
- ✓ Struct fields: Note offsets, don't create fake names
- ✓ All unknowns: Documented with questions

This balances **honesty** (clear about unknowns) with **practicality** (locals are temporary).

