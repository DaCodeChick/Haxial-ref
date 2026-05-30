# Session 2: Enum Correction and ServerState Discovery

## Date
2026-05-30

## Summary
Fixed incorrect MagicNumber enum values and began mapping ServerState structure.

## Issues Found and Corrected

### MagicNumber Enum Values - CRITICAL BUG
**Problem**: Enum contained wrong values due to decimal conversion error

**Before (WRONG)**:
```c
enum MagicNumber {
    CONNECTION_MAGIC = 0x2aa5eaec,  // INCORRECT
    SOCKET_MAGIC     = 0x427d02a5   // INCORRECT
};
```

**After (CORRECT)**:
```c
enum MagicNumber {
    CONNECTION_MAGIC = 0x2aa24aec,  // 715279084 decimal ✓
    SOCKET_MAGIC     = 0x42781ea5   // 1115168421 decimal ✓
};
```

**Verification**:
- `socket_shutdown`: checks `socket->magic == 0x42781ea5` ✓
- `handle_connection_handshake`: checks `connection->magic != 0x2aa24aec` ✓
- Binary search found `ec 4a a2 2a` at 20+ locations ✓

**Actions Taken**:
1. Deleted incorrect MagicNumber enum
2. Recreated with correct decimal values: 715279084 and 1115168421
3. Verified decompilation shows correct hex values
4. Documented correction in `enum-correction.md`

**Status**: ✅ **FIXED** - Enum now matches actual magic values in binary

## New Discoveries

### Global Structure: g_server_state
**Location**: `0x080dde20`  
**Renamed from**: `DAT_080dde20`  
**Type**: ServerState structure (large, 700+ bytes minimum)

**Usage**:
- Passed to `server_initialize(ServerState *server_state)`
- Used throughout server code as main state container
- Contains sockets, buffers, configuration, and runtime state

### ServerState Structure (Partial)
Created initial ServerState struct with known fields:

**Socket Pointers**:
- `+0x00`: `main_socket` - SocketInfo* (from create_socket)
- `+0x0c`: `socket_2` - SocketInfo*
- `+0x10`: field_0x10 - void* pointer

**Data Structures**:
- `+0x34`: `list_or_container` - Populated with list structure
- `+0x48`: field_0x48 - From FUN_08093504 ("Default")
- `+0x4c`: field_0x4c - From FUN_08093054 ("Bases")
- `+0x58-0x64`: Multiple pointers from FUN_0808df34 and FUN_08091020

**Configuration Values**:
- `+0x258`: 0x29cc = 10700 decimal [GUESS: main server port]
- `+0x278`: 0x2ba9 = 11177 decimal [GUESS: tracker-related port]
- `+0x2c8`: 0x100000 = 1048576 decimal [GUESS: buffer/memory limit]

**IDs**:
- `+0x264-0x26c`: Three random uint32 values (from FUN_0807aac0)

**Buffers**:
- `+0x98`: String buffer (256 bytes)
- `+0x2ac`: Pointer to large allocated buffer (0x32000 = 204800 bytes)
- `+0x2b0-0x2b4`: Buffer-related fields

### Issues with Current Struct
The struct definition has alignment/offset issues:
- Ghidra showing accesses like `server_state->gap_0x27a[0x18]`
- Indicates struct layout doesn't match actual memory layout
- Need to rebuild incrementally using set_field approach

## Functions Analyzed

### server_initialize (0x080535b4)
**Updated prototype**: `void server_initialize(ServerState *server_state)`

**Key operations**:
1. Initializes port values (10700, 11177)
2. Generates random ID triplet if not set
3. Allocates 204800-byte buffer
4. Creates multiple data structures (lists, containers)
5. Sets up "General News" default structure
6. Creates two sockets via create_socket
7. Checks setup state - if uninitialized, prompts user

**Strings referenced**:
- "Bases"
- "Default"
- "General News"
- "KDX"
- "Welcome"
- "KDX Server initial setup. Press enter to continue.\n"

### server_main_loop
**Calls**:
1. FUN_08097318()
2. FUN_080967a0()
3. `server_initialize(&g_server_state)`
4. `server_event_loop()`

## Documentation Created

### New Files
1. `enum-correction.md` - Documents MagicNumber enum fix
2. `server-state-discovery.md` - ServerState field mapping
3. `session-2-enum-correction-and-serverstate.md` - This file

### Updated Files
- `enums-created.md` - MagicNumber enum corrected
- Progress tracking documents

## Known Constants

### Confirmed Values
- **Socket magic**: 0x42781ea5
- **Connection magic**: 0x2aa24aec
- **Port 1**: 10700 (0x29cc)
- **Port 2**: 11177 (0x2ba9) - purpose unclear
- **Buffer size**: 204800 bytes (0x32000)

### Unconfirmed
- Whether 10700 is the main server port or tracker port
- What 11177 represents (could be default tracker port)
- Purpose of the 204KB buffer

## Next Steps

### Immediate
1. Fix ServerState struct alignment issues
2. Use set_field to add fields incrementally
3. Verify each field offset before proceeding
4. Re-decompile to check improved output

### Investigation Needed
1. What is FUN_0807aac0? (generates random IDs)
2. What is FUN_08093054? (creates "Bases" structure)
3. What is FUN_08093504? (creates "Default" structure)
4. What is FUN_0808df34? (creates multiple data structures)
5. What is FUN_08091020? (creates structures)
6. What is the "General News" structure? (0xe8 bytes)

### Analysis Continuation
1. Complete server_initialize function renaming
2. Analyze server_event_loop (main server loop)
3. Continue recursive analysis of helper functions
4. Map remaining ServerState fields
5. Identify all port numbers and their purposes
6. Document packet structures when found

## Lessons Learned

### Enum Creation
- Always verify enum values against actual code usage
- Double-check decimal-to-hex conversions
- Use binary search to find all usages of constants
- Verify decompiled code shows expected values

### Large Structure Analysis
- Don't create full struct definition at once
- Use incremental field discovery with set_field
- Verify alignment at each step
- Re-decompile frequently to catch issues early

### Analysis Workflow
- Cross-reference enum values with actual comparisons
- Search for byte patterns to find all usages
- Verify types match usage before applying
- Document uncertainties and guesses clearly

## Statistics

### Corrected
- 1 enum with 2 incorrect values → fixed

### Discovered
- 1 major global structure (g_server_state)
- 15+ struct fields identified (offsets known)
- 3 port/configuration values
- 1 large buffer allocation (204KB)

### Renamed
- 1 global: DAT_080dde20 → g_server_state
- 1 function prototype updated with struct type

### Documented
- 10+ field offsets with purposes
- 5+ unknown helper functions for investigation
- 3 configuration constants
- Multiple questions for future work

## Status

### Completed ✓
- MagicNumber enum corrected
- g_server_state identified and renamed
- Initial ServerState struct created
- server_initialize prototype updated
- Field discovery in progress

### In Progress
- ServerState struct refinement (alignment issues)
- server_initialize complete analysis
- Helper function identification

### Blocked
- None currently

## Open Questions

1. Is 10700 the main server port or tracker port?
2. What is port 11177 used for?
3. What generates the random ID triplet? (FUN_0807aac0)
4. What are "Bases" and "Default" structures?
5. What is the 204KB buffer used for?
6. What is the "General News" structure?
7. How many total fields does ServerState have?
8. Are there multiple ServerState instances or just one global?

## Related Files
- `/home/admin/Documents/GitHub/Haxial-ref/AGENTS.md`
- `/home/admin/Documents/GitHub/Haxial-ref/docs/protocol/findings/enum-correction.md`
- `/home/admin/Documents/GitHub/Haxial-ref/docs/protocol/findings/server-state-discovery.md`
- `/home/admin/Documents/GitHub/Haxial-ref/docs/protocol/findings/enums-created.md`
- `/home/admin/Documents/GitHub/Haxial-ref/docs/protocol/findings/confirmed-constants.md`
