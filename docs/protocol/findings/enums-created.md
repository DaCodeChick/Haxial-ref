# Enums Created for Numerical Constants

## Enums Defined

### 1. KDXErrorCode (uint, 4 bytes)
**Location**: /kdx/KDXErrorCode

```c
enum KDXErrorCode : uint {
    ERROR_INVALID_PARAM      = 0x10002,  // 65538 - NULL pointer, invalid params
    ERROR_INVALID_MAGIC      = 0x10019,  // 65561 - Wrong magic number
    ERROR_OUT_OF_MEMORY      = 0x20065,  // 131173 - malloc() failed
    ERROR_SOCKET_ALREADY_CLOSED = 0x40066, // 262246 - Socket already in closed state
    ERROR_SOCKET_CLOSED      = 0x4006e,  // 262254 - Socket closed error
};
```

**Used in**:
- SocketInfo.error_code field
- log_error() first parameter
- Error handling throughout codebase

**To add more values**:
1. Find log_error() calls: `ghidra_xrefs(function="log_error")`
2. Look at first parameter (error code)
3. Add to enum with descriptive name

### 2. SocketState (byte, 1 byte)
**Location**: /kdx/SocketState

```c
enum SocketState : byte {
    SOCKET_STATE_CLOSED = 0x1c,  // 28 - Socket closed/shutdown
    // More states to be discovered
};
```

**Used in**:
- SocketInfo.socket_state field (offset +0x58)
- State machine checks throughout socket code

**To add more values**:
1. Search for writes to `socket->socket_state` or `*(byte*)(socket + 0x58)`
2. Search for comparisons with socket_state
3. Analyze state machine in connection handling
4. Add each discovered state with descriptive name

### 3. MagicNumber (uint, 4 bytes)
**Location**: /kdx/MagicNumber

```c
enum MagicNumber : uint {
    SOCKET_MAGIC      = 0x42781ea5,  // 1115488933 - SocketInfo magic
    CONNECTION_MAGIC  = 0x2aa24aec,  // 715516652 - ConnectionState magic
};
```

**Used in**:
- SocketInfo.magic field (offset +0x00)
- ConnectionState.magic field (offset +0x00)
- Validation checks throughout

## Struct Fields Updated

### SocketInfo
```c
struct SocketInfo {
    MagicNumber magic;           // +0x00 - Now uses enum!
    uint lock_or_refcount;       // +0x04
    // ...
    KDXErrorCode error_code;     // +0x14 - Now uses enum!
    // ...
    SocketState socket_state;    // +0x58 - Now uses enum!
    byte is_udp;                 // +0x59
};
```

### ConnectionState
```c
struct ConnectionState {
    MagicNumber magic;           // +0x00 - Now uses enum!
    // ... rest TBD
};
```

## Improved Readability Examples

### Before Enums
```c
void socket_shutdown(SocketInfo *socket) {
    if (socket->magic == 0x42781ea5) {
        socket->socket_state = 0x1c;
        if (socket->error_code == 0) {
            socket->error_code = 0x4006e;
        }
    }
}
```

### After Enums
```c
void socket_shutdown(SocketInfo *socket) {
    if (socket->magic == SOCKET_MAGIC) {
        socket->socket_state = SOCKET_STATE_CLOSED;
        if (socket->error_code == 0) {
            socket->error_code = ERROR_SOCKET_CLOSED;
        }
    }
}
```

Much clearer!

## Finding More Enum Values

### For Error Codes

**Method 1: Search for log_error calls**
```
ghidra_xrefs(function="log_error", direction="to", limit=100)
```

Then for each caller, disassemble and look at the PUSH instructions before the call:
```
PUSH 0x4e1      ; Line number
PUSH 0x4e       ; Source file ID
PUSH 0x10002    ; ERROR CODE <-- This one
CALL log_error
```

**Method 2: Search for error code patterns**
Look for patterns like:
- 0x1xxxx - Parameter/validation errors
- 0x2xxxx - Memory errors
- 0x4xxxx - Socket/connection errors
- 0x8xxxx - Other subsystems?

### For Socket States

**Method 1: Search for state assignments**
```
# Look for writes to socket+0x58
# Or search for patterns like:
*(byte*)(socket + 0x58) = VALUE
socket->socket_state = VALUE
```

**Method 2: Analyze state machine**
- Find the main socket state handler function
- Look for switch/if-else on socket_state
- Each case is a state value

**Method 3: Search for state comparisons**
```
# Look for:
if (socket->socket_state == VALUE)
cmp byte ptr [socket+0x58], VALUE
```

### For Magic Numbers

**Method 4: Search for large constants**
```
# Search for 4-byte patterns
# Look for validation checks:
if (*struct == 0xNNNNNNNN)
```

## How to Add Enum Values

### Step 1: Find the value
Use one of the methods above to find a numerical constant.

### Step 2: Determine meaning
- Look at context where it's used
- Check comments or nearby code
- Analyze function purpose
- Cross-reference other usages

### Step 3: Add to enum
```bash
ghidra_types(
    action="create_enum",  # Or update existing
    name="EnumName",
    values={"NEW_VALUE_NAME": decimal_value}
)
```

### Step 4: Document it
Add entry to this file and to unknowns if meaning unclear.

## Next Steps

### High Priority
1. **Find all socket states**
   - Search for all writes to socket_state
   - Map out state machine
   - Add complete enum

2. **Catalog all error codes**
   - Extract from all log_error calls
   - Group by category
   - Add complete enum

3. **Find transaction types**
   - Look for packet type fields
   - Transaction dispatch tables
   - Create TransactionType enum

### Medium Priority
4. **Find parameter types**
   - Parameter type IDs for TLV structures
   - Create ParamType enum

5. **Find connection states**
   - ConnectionState struct likely has state field
   - Create ConnectionState enum

6. **Find file transfer types**
   - File transfer packet types
   - Create FileTransferType enum

## Benefits Achieved

### Readability
- ✓ `SOCKET_STATE_CLOSED` instead of `0x1c`
- ✓ `ERROR_SOCKET_CLOSED` instead of `0x4006e`
- ✓ `SOCKET_MAGIC` instead of `0x42781ea5`

### Type Safety
- ✓ Compiler/Ghidra can check enum usage
- ✓ Shows valid values in decompiler
- ✓ Auto-complete in Ghidra

### Documentation
- ✓ Enum names document meaning
- ✓ Easy to add comments to enum values
- ✓ Centralized constant definitions

### Maintenance
- ✓ Easy to add new values as discovered
- ✓ All usages update automatically
- ✓ Consistent naming across codebase

