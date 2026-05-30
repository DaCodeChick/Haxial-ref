# AGENTS.md - Ghidra MCP Analysis Guidelines for Haxial KDX

## Overview

This document provides guidelines for using Ghidra and the Ghidra MCP (Model Context Protocol) server to analyze Haxial KDX binaries and reverse engineer the network protocol. The goal is to achieve binary-compatible reimplementation by understanding the original protocol through static analysis.

## Purpose

Haxial KDX uses a custom network protocol. To create a compatible implementation, we need to:

1. Understand the packet structure and format
2. Identify all transaction types and their opcodes
3. Document data types (especially Pascal strings)
4. Map out the connection handshake and authentication flow
5. Document all protocol constants and magic numbers

## libhaxial Naming Conventions

When implementing the reimplemented library, follow these naming conventions:

### Functions
- **Prefix**: `hx_` for all public API functions
- **Style**: snake_case
- **Examples**:
  - `hx_sha1_init()`, `hx_sha1_update()`, `hx_sha1_finalize()`
  - `hx_blowfish_init()`, `hx_blowfish_encrypt()`
  - `hx_socket_create()`, `hx_socket_send()`
  - `hx_connection_handshake()`, `hx_connection_send_packet()`

### Constants and Macros
- **Style**: SCREAMING_SNAKE_CASE (all caps with underscores)
- **Examples**:
  - `HX_SOCKET_MAGIC`, `HX_CONNECTION_MAGIC`
  - `HX_ERROR_INVALID_PARAM`, `HX_ERROR_OUT_OF_MEMORY`
  - `HX_SOCKET_STATE_CLOSED`, `HX_SOCKET_STATE_CONNECTED`
  - `HX_DEFAULT_PORT`, `HX_TRACKER_PORT`
  - `HX_MAX_PACKET_SIZE`, `HX_BUFFER_SIZE`

### Types (Structs, Enums, Typedefs)
- **Prefix**: `T` for all types
- **Style**: CamelCase (PascalCase)
- **Examples**:
  - `TBlowfishContext`, `TSha1Context`
  - `TSocketInfo`, `TConnectionState`, `TServerState`
  - `TLinkedListHead`, `TPacketHeader`
  - `TMagicNumber`, `TSocketState`, `TErrorCode`
  - `TPascalString`, `TTransactionType`

### Internal/Private Symbols
- **Prefix**: `hxi_` for internal functions (not part of public API)
- **Style**: snake_case
- **Examples**:
  - `hxi_validate_magic()`, `hxi_buffer_ensure_capacity()`
  - Internal helpers not exposed in public headers

### Example Usage
```c
// Constants
#define HX_SOCKET_MAGIC 0x42781ea5
#define HX_ERROR_SOCKET_CLOSED 0x4006e

// Types
typedef enum {
    HX_SOCKET_STATE_CLOSED = 0x1c,
    HX_SOCKET_STATE_CONNECTED = 0x01
} TSocketState;

typedef struct {
    uint32_t magic;
    int socket_fd;
    TSocketState state;
} TSocketInfo;

// Functions
int hx_socket_create(const char *host, uint16_t port, TSocketInfo **out_socket);
int hx_socket_send(TSocketInfo *socket, const void *data, size_t length);
void hx_socket_shutdown(TSocketInfo *socket);
```

### Rationale
- **hx_ prefix**: Short, unique, clearly identifies libhaxial code
- **snake_case for functions**: C standard library convention, readable
- **CAPS for constants**: Standard C convention, high visibility
- **TCamelCase for types**: Distinct from functions, clear type markers
- Follows modern C library conventions while maintaining clarity

## Target Binaries

Analyze the following KDX binaries in priority order:

1. **KDX Server (Mac)** - Primary analysis target, likely contains complete protocol
2. **KDX Client (Mac)** - Client-side protocol implementation
3. **KDX Client (Windows)** - Cross-reference for platform differences
4. **KDX Tracker** (if available) - Tracker protocol analysis

## Ghidra MCP Setup

### Prerequisites

- Ghidra 11.0 or later
- Ghidra MCP server configured and running
- KDX binaries imported into Ghidra projects

### Project Organization

Create separate Ghidra projects for each binary:
- `haxial_kdx_server_mac.gpr`
- `haxial_kdx_client_mac.gpr`
- `haxial_kdx_client_win.gpr`
- `haxial_kdx_tracker.gpr`

Use consistent analysis settings across all projects:
- Enable all default analyzers
- Enable string analysis with ASCII and UTF-8
- Enable function call analysis
- Enable reference analysis

## Pascal String Detection and Handling

### Overview

**CRITICAL:** KDX uses Pascal strings extensively throughout the codebase. A Pascal string is defined as:

```
[uint8_t length][char data[length]]
```

- **1 byte** denoting the string length (0-255)
- **N bytes** of string data (no null terminator)
- Maximum length: 255 characters

### Identification Patterns

#### Pattern 1: String Table Sections

Look for data sections containing sequential Pascal strings:

```
Offset  | Hex                          | Interpretation
--------|------------------------------|----------------
0x1000  | 0B 48 65 6C 6C 6F 20 57 6F  | length=11, "Hello Wo"
0x100B  | 72 6C 64                     | "rld"
0x100C  | 05 45 72 72 6F 72           | length=5, "Error"
```

Characteristics:
- First byte is reasonable length (1-100 typical)
- Following bytes are printable ASCII (0x20-0x7E)
- Strings appear in sequence
- Often in `.data` or `.rodata` sections

#### Pattern 2: String References in Code

Look for code patterns that access Pascal strings:

**Loading string length:**
```assembly
movzx   eax, byte ptr [string_addr]      ; Load length byte
mov     ecx, eax                          ; Use length
lea     edx, [string_addr+1]              ; Point to data
```

**Comparison operations:**
```assembly
movzx   eax, byte ptr [rdi]               ; Load length from string 1
movzx   ecx, byte ptr [rsi]               ; Load length from string 2
cmp     eax, ecx                          ; Compare lengths
jne     not_equal
```

#### Pattern 3: Network Send/Receive

Pascal strings in network code:
```assembly
mov     al, [string_ptr]                  ; Get length
inc     string_ptr                         ; Skip to data
call    socket_send                        ; Send length + data
```

### Applying Pascal String Data Type in Ghidra

#### Step 1: Create Pascal String Data Type

1. Open Data Type Manager (Window → Data Type Manager)
2. Right-click on project archive → New → Structure
3. Name it `PascalString`
4. Add components:
   - `uint8 length` (size 1)
   - `char data[255]` (size 255) - Ghidra will adjust based on actual length

**Alternative:** Use Ghidra's built-in `pascalstring` data type (case-sensitive)

#### Step 2: Apply to Data

1. Navigate to suspected Pascal string location
2. Right-click → Data → Choose Data Type
3. Select `pascalstring` or your custom `PascalString`
4. Ghidra will parse the length byte and display the string

**Quick application:**
- Position cursor at string address
- Press `T` (define data type)
- Type `pascalstring` and press Enter

#### Step 3: Batch Application

For string tables with many Pascal strings:

1. Create a script or use Ghidra's string search
2. Look for pattern: byte < 128, followed by printable ASCII
3. Apply data type to all matches
4. Review and adjust as needed

### Renaming DAT_ Globals

#### For Pascal String Globals

When you identify a Pascal string global (default name like `DAT_0040a120`):

1. **Understand the string content first**
   - What does the string say? ("Error", "Welcome", "Invalid login", etc.)
   - Where is it used? (error messages, UI text, protocol messages)

2. **Choose a descriptive name** following conventions:
   - Prefix: `g_str_` (global string)
   - Description: Snake_case describing content/purpose
   - Examples:
     - `DAT_0040a120` → `g_str_welcome_message`
     - `DAT_0040a130` → `g_str_error_invalid_login`
     - `DAT_0040a140` → `g_str_server_name`
     - `DAT_0040a150` → `g_str_protocol_version`

3. **Apply the Pascal string data type** (see above)

4. **Add a comment** with:
   - String value in quotes: `"Welcome to KDX Server"`
   - Usage context: `Used in handshake response`
   - Any protocol relevance

#### For Non-String Globals

Identify the type and rename based on usage:

**Numeric Constants:**
- `DAT_0040b000` (uint32 = 0x48544C50) → `g_magic_hotline` (comment: "HTLP magic number")
- `DAT_0040b004` (uint16 = 0x0001) → `g_protocol_version`
- `DAT_0040b008` (uint16 = 5555) → `g_default_port`

**Function Pointer Tables:**
- `DAT_0040c000` (array of function pointers) → `g_transaction_handlers`
- Index by transaction ID in comments

**Data Structures:**
- `DAT_0040d000` (struct) → `g_server_config`
- Define the struct type first, then apply it

**Boolean/Flags:**
- `DAT_0040e000` (uint8) → `g_debug_enabled`
- `DAT_0040e001` (uint32) → `g_server_flags`

**Naming Conventions:**
- Prefix: `g_` (global), `s_` (static), `c_` (constant)
- Type hint: `_str_`, `_magic_`, `_ptr_`, `_tbl_`, `_cfg_`, etc.
- Snake_case for readability
- Be descriptive but concise

### Example Workflow

```
1. Find DAT_0040a120 in global variables
2. Navigate to address 0x0040a120
3. Examine bytes: 0B 48 65 6C 6C 6F 20 57 6F 72 6C 64
4. Recognize pattern: length=11, followed by "Hello World"
5. Apply pascalstring data type
6. Rename: DAT_0040a120 → g_str_hello_world
7. Find XREFS to this global
8. Analyze usage in context
9. Add comment: "Displayed in server startup message"
10. Document in docs/protocol/findings/
```

## Protocol Analysis Workflow

### Phase 1: Identify Network Functions

1. **Locate socket operations:**
   - Search for imports: `socket`, `connect`, `send`, `recv`, `accept`, `listen`
   - On Windows: `WSAStartup`, `WSASocket`, `WSASend`, `WSARecv`
   - Follow call graphs from these functions

2. **Identify wrapper functions:**
   - KDX likely wraps socket calls
   - Look for functions that call socket operations
   - Rename: `FUN_00401234` → `kdx_socket_send`, etc.

3. **Find packet handling:**
   - Look for loops that read from sockets
   - Functions that parse/construct buffers
   - Header processing code

### Phase 2: Packet Structure Analysis

1. **Locate packet construction:**
   - Find functions that build buffers for sending
   - Look for sequential writes of specific sizes
   - Identify header fields

2. **Document header format:**
   - Note offsets and sizes of each field
   - Determine byte order (big-endian typical for network)
   - Identify magic numbers/signatures
   - Document in `docs/protocol/packets.md`

3. **Example header extraction:**
   ```
   Offset +0x00: mov dword [buffer], 0x48544C50    ; Magic "HTLP"
   Offset +0x04: mov word [buffer+4], packet_type  ; Type field
   Offset +0x06: mov word [buffer+6], trans_id     ; Transaction ID
   Offset +0x08: mov dword [buffer+8], total_size  ; Size field
   ```

4. **Create Ghidra struct:**
   - Define `PacketHeader` structure matching discovered format
   - Apply to packet buffer references

### Phase 3: Transaction Type Discovery

1. **Find transaction dispatch:**
   - Look for switch statements or jump tables
   - Transaction ID used as index/selector
   - Function pointer tables

2. **Document each transaction:**
   - Transaction ID (opcode)
   - Transaction name/purpose
   - Request parameters
   - Response format
   - Document in `docs/protocol/transactions.md`

3. **Example transaction handler:**
   ```assembly
   ; Switch on transaction type
   cmp     eax, 0x6B        ; Transaction 0x6B (login)
   je      handle_login
   cmp     eax, 0x6C        ; Transaction 0x6C (chat)
   je      handle_chat
   ```

4. **Naming convention:**
   - `handle_transaction_login` (0x6B)
   - `handle_transaction_chat` (0x6C)
   - Add transaction ID in comment

### Phase 4: Parameter Block Analysis

1. **Identify parameter structure:**
   - Parameters are typically TLV (Type-Length-Value)
   - Find parameter parsing loops
   - Document parameter types

2. **Parameter block format:**
   ```
   [uint16 type][uint16 length][byte data[length]]
   ```

3. **Create parameter type enum:**
   - Document all parameter type IDs
   - Associate with names: `PARAM_USERNAME = 0x0001`

### Phase 5: Authentication Flow

1. **Trace login sequence:**
   - Client → Server: Login request
   - Server → Client: Challenge/response or acceptance
   - Session establishment

2. **Identify credentials:**
   - Username format (Pascal string?)
   - Password handling (plaintext, hashed?)
   - Session tokens

3. **Document handshake:**
   - Step-by-step packet exchange
   - Draw sequence diagram
   - Note any version negotiation

## Cross-Referencing Strategy

### Using XREFs Effectively

1. **From data to code:**
   - Select Pascal string or constant
   - View references (right-click → References → Show References to Address)
   - Analyze each usage context

2. **From code to data:**
   - Identify function that processes data
   - Follow data flow backward
   - Find origin of packet/string data

3. **Function call hierarchy:**
   - Use Function Call Graph
   - Understand high-level flow
   - Identify entry points (main, network loop)

### Finding Protocol Constants

Common locations for protocol constants:
- `.data` section: initialized globals
- `.rodata` section: read-only constants
- Immediate values in code: magic numbers
- String tables: error messages, commands

Search techniques:
- String search: "invalid", "error", "login", "password"
- Sequence search: KDX-specific magic numbers (TBD from analysis)
- Value search: common port numbers (examine network code for defaults)

## Documentation Standards

### Recording Findings

For each discovery, document in `docs/protocol/findings/`:

**Template:**
```markdown
## [Component Name]

**Address:** 0x00401234
**Type:** Function / Data / Structure
**Original Name:** FUN_00401234 / DAT_0040a120
**Renamed:** kdx_send_packet / g_str_welcome

### Description
Brief description of purpose and functionality.

### Details
- Parameter 1: Description
- Parameter 2: Description
- Return value: Description

### Hex Dump (if applicable)
```
00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
```

### Cross References
- Called by: function_x, function_y
- Calls: socket_send, malloc
- References: g_packet_buffer

### Protocol Relevance
How this relates to the network protocol implementation.

### Notes
Additional observations, questions, or areas for further analysis.
```

### Protocol Documentation

Update corresponding protocol docs as you discover details:

- **docs/protocol/packets.md**: Packet format, headers, structure
- **docs/protocol/transactions.md**: Transaction types, IDs, parameters
- **docs/protocol/data-types.md**: Pascal strings, integers, structs
- **docs/protocol/constants.md**: Magic numbers, opcodes, ports
- **docs/protocol/connection.md**: Handshake, authentication, session

## Common Patterns to Look For

### Pattern 1: String Comparison

```assembly
movzx   eax, byte ptr [rdi]      ; Load length of string 1
movzx   ecx, byte ptr [rsi]      ; Load length of string 2
cmp     eax, ecx                 ; Compare lengths
jne     strings_not_equal
lea     rdi, [rdi+1]             ; Skip to data
lea     rsi, [rsi+1]
mov     edx, eax                 ; Length to compare
call    memcmp                   ; Compare string data
```

### Pattern 2: Packet Header Write

```assembly
mov     dword ptr [rbx], MAGIC   ; Write magic
mov     word ptr [rbx+4], dx     ; Write type
mov     word ptr [rbx+6], si     ; Write transaction ID
bswap   eax                      ; Convert to network byte order
mov     dword ptr [rbx+8], eax   ; Write size
```

### Pattern 3: Transaction Dispatch

```assembly
movzx   eax, word ptr [packet+4] ; Read transaction type
cmp     eax, MAX_TRANSACTION
ja      invalid_transaction
lea     rdx, [transaction_table]
mov     rax, qword ptr [rdx + rax*8]  ; Load handler from table
test    rax, rax
jz      unsupported_transaction
call    rax                      ; Call handler
```

### Pattern 4: Pascal String to C String

```assembly
movzx   ecx, byte ptr [rsi]      ; Load Pascal string length
lea     rsi, [rsi+1]             ; Skip to data
mov     rdi, output_buffer
rep     movsb                    ; Copy string data
mov     byte ptr [rdi], 0        ; Add null terminator
```

## Tips and Best Practices

### General Tips

1. **Start with strings:** Error messages and UI strings provide context
2. **Follow the data:** Track how data flows from network to processing
3. **Use bookmarks:** Mark important functions, structures, and data
4. **Take notes:** Document as you go, don't rely on memory
5. **Compare binaries:** Mac vs Windows reveals platform-specific code
6. **Be systematic:** Analyze one subsystem completely before moving on

### Ghidra-Specific Tips

1. **Decompiler view:** Use C pseudocode to understand logic
2. **RENAME IMMEDIATELY:** As soon as you identify what something is, rename it RIGHT THEN
   - Don't defer renaming - do it as you analyze
   - Rename functions, parameters, local variables, globals, all of it
   - Good names compound - each rename makes the next analysis easier
3. **Define structures:** Apply structs to improve decompiler output
4. **Use scripts:** Automate repetitive tasks
5. **Function signatures:** Define parameter types for better analysis
6. **Calling conventions:** Note platform differences (cdecl, stdcall, etc.)

**NAMING PHILOSOPHY:**

The goal of renaming is **clarity and honesty**, not perfection:

1. **Known variables**: Use specific, descriptive names
   - `socket_fd` not `fd`
   - `connection_magic` not `magic`
   - `bytes_sent` not `count`

2. **Partially known variables**: Use descriptive names with [GUESS] markers
   - `lock_or_refcount` [GUESS: could be lock OR refcount]
   - `event_context` [GUESS: assumed event system context]
   
3. **Unknown variables**: Different rules for parameters vs locals
   - **Parameters**: LEAVE AS-IS (`param_1`, `param_2`)
   - **Globals**: LEAVE AS-IS (`DAT_080xxxxx`)
   - **Locals**: Can use generic names (`temp`, `flags`, `result`) - Ghidra auto-renames these
   - Document parameters and globals in `unknowns-to-revisit.md`
   
4. **For struct fields**: Don't name unknown fields
   - When you see `param_1[4]` and know it's socket_fd, rename it
   - When you see `param_1[9]` and don't know what it is, document the offset (+0x24) in unknowns doc
   - Don't create field names - just note the offsets being accessed

5. **The rule**: Only rename when you actually understand what something is (or can make an educated guess marked with [GUESS])

**Why this matters:**
- `param_1` clearly says "unknown parameter"
- `DAT_080xxxxx` clearly says "unknown global"
- Locals with generic names are OK (Ghidra will auto-rename on function changes)
- Noting offsets preserves exact location info without fake names
- Forces honesty about the state of analysis

**CRITICAL RENAMING WORKFLOW:**
1. Analyze a function
2. Identify what parameters/variables/globals do
3. RENAME THEM IMMEDIATELY - don't move on until you do
4. DEFINE STRUCTURES for pointer parameters (don't leave as int*)
5. APPLY STRUCTURE TYPES to parameters immediately
6. Re-decompile to see improved output
7. Continue analysis with clearer code

**STRUCTURE MANAGEMENT:**
1. **Identify Structures Early**: When you see pointer parameters (int*, uint*, etc.) that are clearly structures, create/apply struct types immediately
2. **Create Structures**: Use the ghidra_struct tool to create structures as you discover their layouts
3. **Apply Types**: Use ghidra_variables with action='set_prototype' to update function signatures with correct struct types
4. **Iterative Refinement**: As you discover more fields, use ghidra_struct with action='set_field' to add/update fields
5. **Name Fields Descriptively**: socket_fd, magic_number, connection_state, etc.

**STRUCT CREATION WORKFLOW - STEP BY STEP:**

When you encounter a function with pointer parameters that are clearly structs:

1. **Analyze field accesses** in the decompiled code:
   ```c
   // Example from decompilation:
   param_1[0]     // offset +0x00
   param_1[4]     // offset +0x10 (4 * 4 bytes = 0x10)
   param_1[0x16]  // offset +0x58 (0x16 * 4 = 0x58)
   *(char*)(param_1 + 0x59) // offset +0x59
   ```

2. **Create initial struct** using ghidra_struct with action='create':
   ```
   struct MyStruct {
       uint field_0x00;
       byte gap_0x04[12];
       uint field_0x10;
   };
   ```

3. **Add fields incrementally** as you discover them using action='set_field':
   - Set offset, data_type, and field_name for each discovered field
   - Example: offset=0x58, data_type="byte", field_name="socket_state"

4. **Update function prototype** immediately after creating/updating struct:
   - Use ghidra_variables with action='set_prototype'
   - Change `void func(int *param_1)` → `void func(MyStruct *param_1)`

5. **Re-decompile and verify** - Ghidra will now show struct field names instead of offsets

6. **Propagate struct usage** - All callers/callees will now benefit from the typed struct

**EXAMPLE - Real-world struct discovery:**

```
// Initial decompilation of socket_shutdown:
void FUN_08097c90(int *param_1) {
  if (*param_1 == 0x42781ea5) {           // offset +0x00: magic
    shutdown(param_1[4], 2);               // offset +0x10: socket_fd
    *(char*)(param_1 + 0x16) = 0x1c;      // offset +0x58: socket_state
    if (param_1[5] == 0) {                 // offset +0x14: error_code
      param_1[5] = 0x4006e;
    }
  }
}

// Step 1: Create struct with discovered fields
ghidra_struct(action='create', name='SocketInfo', c_definition='''
struct SocketInfo {
    uint magic;           // +0x00
    byte gap[12];
    uint socket_fd;       // +0x10
    uint error_code;      // +0x14
    byte gap2[64];
    byte socket_state;    // +0x58
};
''')

// Step 2: Rename variables
ghidra_batch_rename([{
  "identifier": "FUN_08097c90",
  "new_name": "socket",
  "target_type": "variable",
  "variable_name": "param_1"
}])

// Step 3: Set prototype with struct type
ghidra_variables(action='set_prototype',
                 function_address='0x08097c90',
                 prototype='void socket_shutdown(SocketInfo *socket)')

// Step 4: Rename the function
ghidra_rename_symbol(identifier='FUN_08097c90',
                     new_name='socket_shutdown',
                     target_type='function')

// Result: Clean, typed decompilation
void socket_shutdown(SocketInfo *socket) {
  if (socket->magic == 0x42781ea5) {
    shutdown(socket->socket_fd, 2);
    socket->socket_state = 0x1c;
    if (socket->error_code == 0) {
      socket->error_code = 0x4006e;
    }
  }
}
```

**STRUCT FIELD NAMING CONVENTIONS:**
- Use descriptive names: `socket_fd` not `fd`, `error_code` not `err`
- Use snake_case: `bytes_sent` not `bytesSent`
- Prefix flags/state: `is_connected`, `has_data`, `socket_state`
- Arrays: `buffer[256]` or `s_box_0[256]` (if multiple boxes)
- Pointers: `send_queue_head`, `next_packet`

**FUNCTION NAMING:**
- Functions are actions: `handle_connection_handshake`, `parse_packet_header`, `send_login_reply`
- Structures are nouns: `SocketInfo`, `ConnectionState`, `PacketHeader`
- Avoid naming functions like structures: "connection_state_machine" → "handle_connection_state" or "process_connection_handshake"

**RECURSIVE ANALYSIS:**
When analyzing a function:
1. Rename the function itself
2. **RENAME ALL PARAMETERS** - Every single param_1, param_2, etc. must be renamed
3. **RENAME ALL LOCAL VARIABLES** - Every single local_*, iVar*, uVar*, etc. must be renamed
4. Define/apply struct types for pointers
5. Look at ALL called functions - rename and analyze them too (recursively apply steps 1-4)
6. Look at ALL caller functions - understand the context
7. Update documentation as you go
8. Only then move to the next function

**ZERO TOLERANCE FOR UNNAMED VARIABLES:**
- NO param_1, param_2, param_3, etc. should remain IF you know what they are
- NO local_* variables should remain IF you know what they are
- NO iVar*, uVar*, pvVar*, etc. should remain IF you know what they are
- NO int* for structures - must be StructName*
- If you DON'T know what something is yet:
  - **Parameters**: LEAVE AS-IS (`param_1`, `param_2`)
  - **Globals**: LEAVE AS-IS (`DAT_080xxxxx`)
  - **Locals**: Can use generic names (`temp`, `flags`, `result`) - Ghidra auto-renames on function changes
- Document unknown parameters and globals in unknowns-to-revisit.md with the offset/usage

**CRITICAL: DOCUMENTING UNKNOWNS**

When you encounter variables/fields you don't understand yet:

1. **LEAVE THEM AS-IS** - Keep param_1, uVar2, local_8c, etc.
2. **IMMEDIATELY document** in `docs/protocol/findings/unknowns-to-revisit.md`
3. **Include**:
   - Function name and address
   - Variable as-is (param_1, uVar2, etc.) or offset (+0x24)
   - What you DON'T know about it
   - How it's used in the code
   - Questions to answer later
   - Cross-references to related unknowns

**Examples of what to document:**

```markdown
### socket_send_or_queue (0x08098104)
- **param_1[9]** (offset +0x24) - What is this? Appears to be queue head
  - Used: Added to by FUN_0806edd8, checked against 0
  - Question: What is the queue entry structure?
  - Related: FUN_08098d3c (allocates entry), FUN_08098ea0 (flushes queue)

- **uVar1** - What does this represent?
  - Used: Result of send(), compared with param_3
  - Question: Is this bytes sent, or something else?
  - Related: Assigned to param_1[0x11] when partial send
```

**Do NOT:**
- Invent placeholder names like temp1, temp2, flags1, flags2
- Name struct fields you don't understand (field_0x08, unknown_data, etc.)
- Assume you understand something without verifying

**DO:**
- Leave variables as param_1, uVar2, local_8c when unknown
- Document the offset being accessed (param_1[9] = offset +0x24)
- Note what needs to be revisited
- Cross-reference related unknowns
- Update the unknowns document as you learn more

**Note on local variables:**
- Locals can use generic names (temp, flags, result) since Ghidra auto-renames on function changes
- Parameters and globals should remain as param_1, DAT_* until understood

**Struct Gaps:**

When creating structs with unknown fields:

```c
struct SocketInfo {
    uint magic;                    // +0x00 - KNOWN: 0x42781ea5
    uint lock_or_refcount;         // +0x04 - GUESS: lock or refcount?
    byte gap_0x08[8];              // +0x08 - UNKNOWN: document in unknowns-to-revisit.md
    uint socket_fd;                // +0x10 - KNOWN: file descriptor
};
```

**Then document:**
```markdown
### SocketInfo Structure Gaps
- **gap_0x08[8]** - 8 bytes between lock_or_refcount and socket_fd
  - Accesses seen: param_1[2] in function XYZ
  - Candidates: event_context pointer? flags? padding?
  - TODO: Check all functions using SocketInfo for accesses to +0x08 to +0x0F
```

**TYPE CORRECTION WORKFLOW:**
1. Rename variable/parameter with meaningful name
2. Correct the type immediately if it's a struct pointer (int* → ConnectionState*)
3. Use set_prototype to update entire function signature at once
4. Verify types are correct in decompilation
5. **Document any unknowns/assumptions in unknowns-to-revisit.md**

### Reverse Engineering Strategy

1. **Top-down approach:**
   - Start from main()
   - Identify major subsystems
   - Drill down into details

2. **Bottom-up approach:**
   - Start from socket operations
   - Work up to high-level functions
   - Build understanding of primitives first

3. **Middle-out approach:**
   - Find interesting functions (login, chat, file transfer)
   - Analyze related code
   - Expand outward in both directions

Use all three approaches as needed for different components.

## Ghidra MCP Integration

### Using Ghidra MCP Server

Once the MCP server is configured:

1. **Query functions:** Ask for function signatures, parameters, return types
2. **Analyze data structures:** Get struct definitions
3. **Search patterns:** Find code patterns across the binary
4. **Cross-reference analysis:** Query all references to a symbol
5. **Script execution:** Run analysis scripts via MCP

### MCP Commands (Examples)

These are conceptual - actual MCP interface may vary:

```
# Get function information
mcp.get_function("0x00401234")

# Find all references to a global
mcp.find_xrefs("g_str_welcome_message")

# Search for byte pattern
mcp.search_bytes("48 54 4C 50")

# List all Pascal strings in .data section
mcp.find_strings(type="pascal", section=".data")

# Get struct definition
mcp.get_struct("PacketHeader")
```

### Automating Analysis

Use MCP to automate repetitive tasks:
- Batch renaming of similar functions
- Finding all transaction handlers
- Extracting all string constants
- Mapping parameter types

## Validation and Verification

### Cross-Check Your Findings

1. **Multiple binaries:** Compare Mac and Windows implementations
2. **Network captures:** If available, verify against packet captures
3. **Consistency checks:** Ensure packet sizes match specifications
4. **Test hypotheses:** Implement suspected protocol, test with original binaries

### Document Uncertainties

If unsure about something:
- Mark with `[UNCERTAIN]` in documentation
- Note alternative interpretations
- Revisit after gathering more context
- Test with implementation to verify

## Progress Tracking

### Analysis Checklist

- [ ] Identified network send/receive functions
- [ ] Documented packet header format
- [ ] Found all Pascal strings in globals
- [ ] Renamed significant DAT_ globals
- [ ] Mapped transaction type IDs
- [ ] Documented authentication flow
- [ ] Analyzed parameter block structure
- [ ] Extracted protocol constants
- [ ] Documented file transfer protocol
- [ ] Analyzed chat message handling
- [ ] Documented tracker protocol (if separate)
- [ ] Cross-referenced Mac and Windows implementations

### Per-Binary Analysis Status

Track in `docs/protocol/findings/analysis-status.md`:

**KDX Server (Mac):**
- Analysis started: [DATE]
- Network functions: [COMPLETE/IN_PROGRESS/TODO]
- Packet format: [COMPLETE/IN_PROGRESS/TODO]
- Transactions: [X/Y documented]
- Pascal strings: [X identified, Y renamed]

(Repeat for each binary)

## Questions and Support

When stuck or unsure:

1. Document the question in analysis notes
2. Mark the area for further investigation
3. Continue with other components
4. Return with fresh perspective or new context
5. Consult with team members

## Conclusion

This guide provides a systematic approach to reverse engineering the Haxial KDX protocol using Ghidra. Key points:

- **Pascal strings are critical** - identify and mark them properly
- **Rename everything** - good names enable faster analysis
- **Document thoroughly** - your future self will thank you
- **Be systematic** - cover all components methodically
- **Validate findings** - cross-check and test your understanding

The goal is a complete, accurate protocol specification that enables binary-compatible reimplementation. Take your time, be thorough, and document everything.

Happy reverse engineering!
