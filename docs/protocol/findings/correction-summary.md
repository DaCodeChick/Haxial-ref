# Critical Correction Summary

## What Was Wrong

I violated the naming policy by creating placeholder names that hid gaps in understanding:

### Variables
- ❌ Renamed `param_1` → `ctx` (Blowfish - this was OK, I knew what it was)
- ❌ Renamed `uVar1` → `left` (Blowfish - OK)
- ❌ Renamed `uVar3` → `temp1` (Blowfish - WRONG, didn't know what it was)
- ❌ Renamed `uVar4` → `temp2` (Blowfish - WRONG)
- ❌ Renamed `uVar5` → `temp3` (Blowfish - WRONG)
- ❌ Renamed `iVar6` → `f_result` (Blowfish - WRONG)
- ❌ Renamed `uVar1` → `flags` (socket_set_options - WRONG)
- ❌ Renamed `extraout_EAX` → `extra_flags` (socket_set_options - WRONG)

### Struct Fields
- ❌ Would have created `field_0x08` style names (prevented by discussion)

## What's Now Correct

### Policy Updated in AGENTS.md

**NAMING PHILOSOPHY** section now says:
1. **Known** → Use specific names (socket_fd, bytes_sent)
2. **GUESS** → Use descriptive + [GUESS] (lock_or_refcount [GUESS])
3. **UNKNOWN** → LEAVE AS-IS (keep param_1, uVar2, local_8c)
4. **Struct fields** → Don't name unknowns, just note offsets

**ZERO TOLERANCE** section now says:
- NO param_1, uVar2 should remain **IF you know what they are**
- **If you DON'T know**, LEAVE THEM AS-IS
- Document in unknowns-to-revisit.md

**DOCUMENTING UNKNOWNS** section now says:
- Leave as param_1, uVar2, local_8c
- Document offset being accessed (param_1[9] = +0x24)
- Don't invent placeholder names

## The Core Insight

**Placeholder names like temp1, flags, result are WORSE than param_1, uVar2**

Why:
- `param_1` clearly signals "I don't know yet"
- `temp1` pretends to be a name but means nothing
- `temp1` hides ignorance behind a fake label
- `param_1` makes it obvious what needs investigation

## What I Should Have Done

### For Blowfish Variables
```c
// Instead of:
void blowfish_encrypt(BlowfishContext *ctx, uint *input_block, byte *output_block) {
    uint left, right, temp1, temp2, temp3;  // WRONG
    int f_result, f_result2;                 // WRONG
}

// Should have done:
void blowfish_encrypt(BlowfishContext *ctx, uint *input_block, byte *output_block) {
    uint left, right, uVar3, uVar4, uVar5;  // Leave unknowns as-is
    int iVar6, iVar7;                        // Leave unknowns as-is
}
// Then document: "uVar3, uVar4, uVar5 - intermediate values in Feistel rounds, exact role unclear"
```

### For socket_set_options Variables
```c
// Instead of:
void socket_set_options(SocketInfo *socket) {
    uint flags, temp_flags;     // WRONG
    int result, extra_result;   // WRONG
}

// Should have done:
void socket_set_options(SocketInfo *socket) {
    uint uVar1, uVar3;          // Leave as-is
    int iVar2;                   // Leave as-is
    // extraout_EAX variables can stay or be left as-is
}
// Then document: "uVar1 - fcntl flags, need to determine exact meaning"
```

### For Struct Fields
```c
// Don't create field names for unknowns
struct SocketInfo {
    uint magic;                    // +0x00 - KNOWN
    uint lock_or_refcount;         // +0x04 - GUESS
    byte gap_0x08[8];              // +0x08 - Document accesses at this offset
    uint socket_fd;                // +0x10 - KNOWN
};
```

## Three-Tier System (Final)

| Tier | Approach | Example |
|------|----------|---------|
| **KNOWN** | Rename with specific name | `socket_fd`, `bytes_sent` |
| **GUESS** | Rename with [GUESS] marker | `lock_or_refcount [GUESS]` |
| **UNKNOWN** | Leave as-is, document offset | Keep `param_1`, document "+0x24" |

## Files Updated

1. **AGENTS.md**:
   - NAMING PHILOSOPHY - clarified to leave unknowns as-is
   - ZERO TOLERANCE - added "IF you know what they are"
   - DOCUMENTING UNKNOWNS - removed placeholder name examples
   - Struct gaps - show offset documentation approach

2. **unknowns-checklist.md**:
   - Updated to reflect "leave as-is" approach
   - Removed placeholder name guidance
   - Added examples of documenting param_1, uVar2

3. **naming-policy-clarification.md**:
   - Complete explanation of the correct approach
   - Examples of what NOT to do
   - Examples of what TO do

## Key Takeaway

**The goal is visibility of ignorance, not hiding it behind placeholder names**

- `param_1` = "I don't know yet" ✓ (honest)
- `temp1` = "I named this temp" ✗ (hiding ignorance)

Leaving variables as param_1, uVar2 makes it crystal clear what still needs investigation.

