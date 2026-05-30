# SHA1 Implementation Analysis

## Issue Found
SHA1Context struct was created but is **not actually used** in the code.

## How SHA1 Actually Works in KDXServer

### sha1_hash (convenience wrapper)
**Signature**: `void sha1_hash(byte *data, uint length, uint *digest)`

**Implementation**:
1. Creates local context on stack: `uint ctx[22]` + `undefined4 padding` = 92 bytes
2. Initializes SHA1 state values in ctx[0-4]
3. Sets ctx[5] = 0 (block counter)
4. Calls `sha1_update(ctx, data, length)`
5. Calls `sha1_finalize(ctx, digest)`

**Key point**: The context is an **untyped array**, not a struct instance.

### sha1_update (processes data)
**Signature**: `void sha1_update(uint *ctx, uint *data, uint length)`

**Context layout** (inferred from accesses):
- `ctx[0-4]`: SHA1 state (h0-h4) - 20 bytes
- `ctx[5]`: Block counter - 4 bytes
- `ctx[6...]`: 64-byte buffer starts here
- `ctx[0x16]` (ctx[22]): Buffer position (0-64) - 4 bytes

**Total**: 23 uints = 92 bytes

### sha1_finalize (produces digest)
**Signature**: `void sha1_finalize(uint *ctx, uint *digest)`

Finalizes the hash and writes the 20-byte digest output.

## Context Layout

The SHA1 context is **not a formal struct**, but an array with this layout:

```
Offset  | Index | Field            | Size
--------|-------|------------------|------
+0x00   | [0]   | state[0] (h0)    | 4
+0x04   | [1]   | state[1] (h1)    | 4
+0x08   | [2]   | state[2] (h2)    | 4
+0x0c   | [3]   | state[3] (h3)    | 4
+0x10   | [4]   | state[4] (h4)    | 4
+0x14   | [5]   | block_count      | 4
+0x18   | [6]   | buffer[0-63]     | 64
+0x58   | [22]  | buffer_pos       | 4
--------|-------|------------------|------
Total: 92 bytes
```

## Why SHA1Context Struct Exists

The struct was created during analysis but:
1. **sha1_hash** doesn't use it - uses local uint array
2. **sha1_update** takes `uint*` not `SHA1Context*`
3. **sha1_finalize** takes `uint*` not `SHA1Context*`

The struct may have been created for documentation purposes or mistakenly.

## Should SHA1Context Be Kept?

### Option 1: Delete It
**Pros**:
- More honest - not actually used in code
- Avoids confusion about struct vs array
- Follows "only document what exists" principle

**Cons**:
- Loses documentation of context layout
- Can't apply to improve decompiler output (functions take uint*)

### Option 2: Keep It
**Pros**:
- Documents the implicit layout
- Could theoretically be applied to sha1_update/finalize params
- Useful reference for understanding the algorithm

**Cons**:
- Misleading - suggests struct is used when it isn't
- Functions weren't designed to use a struct type

## Current Status

### Renamed Functions
- `FUN_0807bbfc` → `sha1_hash` ✓
- `FUN_0807bc9c` → `sha1_update` ✓  
- `FUN_0807bd98` → `sha1_finalize` ✓

### SHA1Context Struct
- Created at `/crypto/SHA1Context`
- 96 bytes (has padding despite "packed" flag)
- **Not applied to any function parameters**
- **Not used anywhere in actual code**

### Function Signatures
```c
void sha1_hash(byte *data, uint length, uint *digest);
void sha1_update(uint *ctx, uint *data, uint length);
void sha1_finalize(uint *ctx, uint *digest);
```

Note: All take `uint*` not `SHA1Context*`

## Recommendation

**Keep SHA1Context as documentation** but:
1. Add comment explaining it's not used as a formal type
2. Note that functions use `uint*` arrays instead
3. Document the layout for understanding purposes
4. Do NOT apply it to function parameters (would be misleading)

## Updated Documentation

SHA1Context should be documented as:

```c
/**
 * SHA1 context layout (DOCUMENTATION ONLY)
 * 
 * This struct documents the implicit layout of the uint[] arrays
 * used by sha1_update and sha1_finalize. The actual code does NOT
 * use this struct type - it uses untyped uint* pointers.
 * 
 * See sha1_hash() for local context allocation as uint ctx[22].
 */
struct SHA1Context {
    uint state[5];        // +0x00: SHA-1 state (h0-h4)
    uint block_count;     // +0x14: blocks processed
    byte buffer[64];      // +0x18: input buffer  
    uint buffer_pos;      // +0x58: position in buffer (0-64)
};
// Total: 92 bytes (as uint[22] + padding)
```

## Related Files
- sha1_hash: 0x0807bbfc
- sha1_update: 0x0807bc9c  
- sha1_finalize: 0x0807bd98
- SHA1Context struct: /crypto/SHA1Context (96 bytes with padding)
