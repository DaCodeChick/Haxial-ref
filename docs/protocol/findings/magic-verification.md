# Magic Number Verification

## Current Values

### Socket Magic: 0x42781ea5
**Decimal**: 1115488933  
**Hex bytes (little-endian)**: a5 1e 78 42

**Usage verified**:
- socket_shutdown: Checks `socket->magic == 0x42781ea5`
- socket_send_or_queue: Checks `socket->magic == 0x42781ea5`
- Multiple socket validation checks

### Connection Magic: 0x2aa24aec
**Decimal**: 715516652  
**Hex bytes (little-endian)**: ec 4a a2 2a

**Usage verified**:
- handle_connection_handshake: Checks `connection->magic != 0x2aa24aec`
- Found in 20+ locations in binary

## Verification Needed

Please specify what seems incorrect about CONNECTION_MAGIC = 0x2aa24aec?

Options to check:
1. Wrong byte order (should it be big-endian)?
2. Wrong value entirely (what should it be)?
3. Wrong name (should it be called something else)?
4. Wrong usage (not actually a magic number)?

## How to Verify

### Method 1: Check initialization
Look for where connection structures are created/initialized.
The magic should be written at offset +0x00.

### Method 2: Check all comparisons
Search for comparisons with this value.
Should always be validating a ConnectionState struct.

### Method 3: Disassemble comparison sites
Look at the assembly for:
```
cmp dword ptr [connection], 0x2aa24aec
```

### Method 4: Check struct layout
Verify ConnectionState has magic at offset +0x00.

