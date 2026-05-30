# ServerState Structure Discovery

## Overview
The main server state is stored in global `g_server_state` at `0x080dde20`.
This structure is passed to `server_initialize()` and throughout the server code.

## Known Field Accesses

### Socket Pointers
- `+0x00` (param[0]): `main_socket` - SocketInfo* from create_socket(..., 1)
- `+0x0c` (param[3]): `socket_2` - SocketInfo* from create_socket(..., 2)  
- `+0x10` (param[4]): void* from FUN_08091020

### Data Structure Pointers
- `+0x34` (param[0xd]): `list_or_container` - checked for NULL, populated with list structure
- `+0x48` (param[0x12]): from FUN_08093504 (related to "Default")
- `+0x4c` (param[0x13]): from FUN_08093054 (related to "Bases")
- `+0x58` (param[0x16]): from FUN_0808df34 - checked by FUN_0808dfa0
- `+0x5c` (param[0x17]): from FUN_0808df34
- `+0x60` (param[0x18]): from FUN_0808df34
- `+0x64` (param[0x19]): from FUN_08091020

### Configuration Values
- `+0x258` (param[0x96]): Port value `0x29cc` = 10700 decimal [GUESS: main port?]
- `+0x278` (param[0x9e]): Port value `0x2ba9` = 11177 decimal [GUESS: tracker port?]
- `+0x2c8` (param[0xb2]): Value `0x100000` = 1048576 decimal [GUESS: buffer size?]

### IDs / Random Values
- `+0x264` (param[0x99]): Random ID part 1 (from FUN_0807aac0)
- `+0x268` (param[0x9a]): Random ID part 2 (from FUN_0807aac0)
- `+0x26c` (param[0x9b]): Random ID part 3 (from FUN_0807aac0)

### Buffers
- `+0x98` (param + 0x26): String buffer - receives data from FUN_0807a130
- `+0x2ac` (param[0xab]): Pointer to allocated buffer (0x32000 bytes = 204800 bytes)
- `+0x2b0` (param[0xac]): Buffer field 1 (set to 0)
- `+0x2b4` (param[0xad]): Buffer field 2 (set to 0)

## Issues with Current Struct Definition
The ServerState struct created has incorrect alignment/offsets.
Ghidra shows accesses like `server_state->gap_0x27a[0x18]` which indicates misalignment.

Need to:
1. Re-examine actual byte offsets more carefully
2. Consider padding/alignment requirements
3. May need to analyze field-by-field rather than creating full struct at once

## Next Steps
- Continue identifying all field accesses
- Build struct incrementally using set_field
- Verify each field before adding next
- Look for other functions that access g_server_state for more field info
