# Numerical Constants Review

## Constants Identified in Analysis

### Socket States (byte at SocketInfo+0x58)
- **0x1c** - Closed/shutdown state (seen in socket_shutdown)
- **Others**: Unknown, need to find all state values

### Error Codes (uint at SocketInfo+0x14)
- **0x4006e** - Socket closed error
- **0x10002** - Invalid parameter (seen in allocate_memory)
- **0x10019** - Invalid magic number
- **0x40066** - Socket already closed
- **0x20065** - Memory allocation failure
- **Others**: Many more error codes used throughout

### Magic Numbers
- **0x42781ea5** - SocketInfo magic (uint at +0x00)
- **0x2aa24aec** - ConnectionState magic (uint at +0x00)

### Packet/Cipher Constants
- **0x4878** - Cipher constant in handshake

### File/Source Constants (used in log_error calls)
- **0x4e** - Appears frequently as source file identifier
- **0x20065** - Source file identifier
- **0x4d5343** - Another identifier
- Many line numbers used as second parameter

### Socket Options Constants
- **0x800** - O_NONBLOCK flag (fcntl)
- **0x20000** - 128KB buffer size (SO_SNDBUF/SO_RCVBUF)

## Where These Should Become Enums

### 1. Socket States
Should be enum at SocketInfo+0x58:
```c
enum SocketState : byte {
    SOCKET_STATE_CLOSED = 0x1c,
    // Need to find other states
};
```

### 2. Error Codes
Should be enum for error_code field:
```c
enum KDXErrorCode : uint {
    ERROR_INVALID_PARAM = 0x10002,
    ERROR_INVALID_MAGIC = 0x10019,
    ERROR_OUT_OF_MEMORY = 0x20065,
    ERROR_SOCKET_CLOSED = 0x4006e,
    ERROR_ALREADY_CLOSED = 0x40066,
    // Many more to find
};
```

### 3. Magic Numbers
Could be #define or enum:
```c
enum MagicNumbers : uint {
    SOCKET_MAGIC = 0x42781ea5,
    CONNECTION_MAGIC = 0x2aa24aec,
};
```

## How to Find All Values

### For Socket States
1. Search for all writes to `*(char*)(socket + 0x58)`
2. Search for all comparisons with `socket->socket_state`
3. Look at state machine in connection handling

### For Error Codes
1. Search for all writes to `socket->error_code` or `param_1[5]`
2. Search for all calls to `log_error` and extract first parameter
3. Look for error code constants in .rodata

### For Source File IDs
1. Extract all first parameters to `log_error` calls
2. Map them to Pascal strings that might be filenames
3. Create mapping of ID → filename

## Action Items

1. **Search for socket state values** - Find all state assignments
2. **Catalog error codes** - Find all log_error calls, extract codes
3. **Create enums in Ghidra** - Use ghidra_types tool
4. **Apply enums to fields** - Update SocketInfo.socket_state, error_code types
5. **Document meanings** - Add comments for each enum value

