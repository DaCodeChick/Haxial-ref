# Local Variables Clarification

## The Exception for Local Variables

**Local variables can use generic/temp names because Ghidra auto-renames them on certain changes.**

## The Three Categories

### 1. Parameters - NO Generic Names
- **Rule**: LEAVE AS-IS when unknown
- **Why**: Parameters are part of function signature
- **Reason**: param_1 clearly signals "unknown parameter"
- **Example**: Keep `param_1`, `param_2` until understood

```c
// DON'T DO THIS:
void function(int socket, int temp, int flags) {...}

// DO THIS:
void function(int param_1, int param_2, int param_3) {...}
// Then document: "param_2 - purpose unclear, used in calculation"
```

### 2. Globals - NO Generic Names
- **Rule**: LEAVE AS-IS when unknown
- **Why**: Globals are permanent, visible across codebase
- **Reason**: DAT_* clearly signals "uninvestigated global"
- **Example**: Keep `DAT_080db6e8` until understood

```c
// DON'T DO THIS:
int g_callback_1;  // Haven't analyzed what it's for

// DO THIS:
int DAT_080db6e8;  // Obvious it needs investigation
// Then document: "DAT_080db6e8 - set to FUN_08092988, purpose unclear"
```

### 3. Locals - Generic Names OK
- **Rule**: Can use temp, flags, result, etc.
- **Why**: Ghidra auto-renames locals on function changes
- **Reason**: Not permanent, will be updated by Ghidra
- **Example**: Can use `temp`, `result`, `index`

```c
// THIS IS OK:
void function(int param_1, int param_2) {
    int temp = param_1 + param_2;  // OK - local variable
    int result = temp * 2;         // OK - local variable
    int flags = result & 0xFF;     // OK - local variable
}
```

## Why This Distinction?

### Ghidra's Auto-Rename Behavior

When you change a function (signature, add locals, etc.), Ghidra will:
- ✓ Auto-rename local variables (local_8, local_c, etc.)
- ✓ Adjust local variable names based on stack layout
- ✗ NOT change parameter names (param_1 stays param_1)
- ✗ NOT change global names (DAT_* stays DAT_*)

This means:
- Local generic names are temporary
- Parameter/global names are permanent
- Locals will be regenerated anyway
- Parameters/globals need explicit tracking

## Practical Guidelines

### For Parameters (Document Unknowns)
```c
void socket_send_or_queue(int *param_1, undefined4 *param_2, uint param_3)
```
Document in unknowns-to-revisit.md:
- param_1[9] offset +0x24 - appears to be queue head
- param_2 - pointer to buffer pointer
- param_3 - buffer length

### For Locals (Generic Names OK)
```c
void socket_send_or_queue(SocketInfo *socket, void **buffer_ref, uint length) {
    void *buffer;           // OK
    uint bytes_sent;        // OK - known
    int result;             // OK
    uint temp;              // OK
    int flags;              // OK
}
```

### For Globals (Document Unknowns)
```c
// In code:
DAT_080db6e8 = FUN_08092988;
DAT_080db6ec = FUN_08092990;
```
Document in unknowns-to-revisit.md:
- DAT_080db6e8 - set to function address, callback for what?
- DAT_080db6ec - another callback, relationship to DAT_080db6e8?

## Updated Policy Summary

| Variable Type | Unknown Handling | Generic Names? | Document? |
|--------------|------------------|----------------|-----------|
| **Parameters** | Leave as param_1 | ❌ NO | ✓ YES |
| **Globals** | Leave as DAT_* | ❌ NO | ✓ YES |
| **Locals** | Can use temp/flags/result | ✓ YES | Optional |

## Examples from Our Analysis

### Parameters - Correctly Left As-Is
```c
void blowfish_encrypt(BlowfishContext *ctx, uint *input_block, byte *output_block) {
    uint left, right;      // Known - renamed
    uint uVar3, uVar4, uVar5;  // Unknown - left as-is, should document
}
```

Should document:
- uVar3, uVar4, uVar5 - intermediate values in Feistel rounds, exact purpose unclear

### Locals - Generic Names Were OK
```c
void socket_set_options(SocketInfo *socket) {
    uint flags;            // OK - local variable
    int result;            // OK - local variable  
    uint temp_flags;       // OK - local variable
}
```

These were actually fine! Ghidra will rename them on function changes.

### Globals - Correctly Reverted
```c
// Before (WRONG):
g_callback_1 = FUN_08092988;

// After (CORRECT):
DAT_080db6e8 = FUN_08092988;
// Documented: "DAT_080db6e8 - set to FUN_08092988, analyze function to understand"
```

## Revised Assessment

### What We Should Have Done

**For blowfish_encrypt locals:**
```c
// What I did:
uint left, right, temp1, temp2, temp3;  // temp1-3 are locals - ACTUALLY OK!

// What I thought was wrong:
// "Should leave as uVar3, uVar4, uVar5"

// Actually correct:
// temp1-3 are fine for locals, they're temporary names
// BUT: Should still document what we don't understand about them
```

**The real issue:**
- Not that I used temp1/temp2/temp3 for locals (that's OK)
- But that I didn't document what I don't understand about them
- Should have noted: "temp1-3 are intermediate Feistel values, exact role unclear"

### What Should Be Documented

Even though locals can have generic names, we should still document what we don't understand:

```markdown
### blowfish_encrypt (0x0806565c) - Local Variables
- **temp1, temp2, temp3** - Intermediate values in Feistel rounds
  - Used: Appear to be working variables for cipher rounds
  - Question: Why three temps? What's the exact transform happening?
  - TODO: Study Blowfish algorithm to map to standard implementation
```

## Conclusion

**Clarified Policy:**
1. **Parameters**: Must stay param_1 when unknown, document
2. **Globals**: Must stay DAT_* when unknown, document  
3. **Locals**: Can use temp/flags/result, still document unknowns

**The key insight:**
- Locals using temp names is OK (Ghidra auto-renames)
- But we should still document what we don't understand
- The issue wasn't the temp names, it was lack of documentation

**For our previous work:**
- Local variable renames (temp1, flags, result) were actually fine
- But should have documented the unknowns anyway
- Global and parameter renames were correctly reverted

