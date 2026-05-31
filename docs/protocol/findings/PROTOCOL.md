# KDX Protocol Specification

## Packet Format

### Packet Header

**Magic**: `PXTP` (0x50 0x58 0x54 0x50)  
**Size**: Unknown (at least 12 bytes)

```
Offset | Size | Field | Description
-------|------|-------|------------
+0x00  | 4    | magic | "PXTP" packet magic
+0x04  | ?    | ?     | Unknown fields
```

### Validation
- `validate_packet_header` checks magic, sequence numbers, state
- Returns error codes 0x40071-0x40075 for protocol violations

---

## Magic Numbers

### Structure Magic Numbers
- **SocketInfo**: 0x42781ea5
- **ConnectionState**: 0x2aa24aec

### Packet Magic
- **PXTP**: 0x50585450 (packet header)

### Cipher Constants
- **Handshake cipher**: 0x4878 (used in connection_send_handshake_packet)

---

## Connection Handshake

### Handshake States

| Value | State Name | Description |
|-------|------------|-------------|
| 0x01  | INITIAL    | Initial handshake |
| 0x03  | RETRY      | Retry handshake |
| 0x04  | AUTHENTICATED | Handshake complete, authenticated |
| 0x08  | FAILED     | Handshake failed |

### Handshake Flow
1. Client connects → handshake_state = INITIAL (0x01)
2. Server validates → calls authentication callback
3. Success → handshake_state = AUTHENTICATED (0x04)
4. Failure → handshake_state = FAILED (0x08)
5. Retry → handshake_state = RETRY (0x03)

### Functions
- `handle_connection_handshake`: Main state machine
- `connection_send_handshake_packet`: Send handshake packet
- `connection_send_handshake_response`: Send response

---

## Error Codes (KDXErrorCode)

### System Errors (0x1xxxx)
- 0x10003: ERROR_NO_SUCH_PROCESS
- 0x1000d: ERROR_PERMISSION_DENIED
- 0x10016: ERROR_RESOURCE_BUSY
- 0x10056: ERROR_INVALID_PARAM

### Memory Errors (0x2xxxx)
- 0x2000c: ERROR_OUT_OF_MEMORY
- 0x2000e: ERROR_BAD_ADDRESS

### Filesystem Errors (0x3xxxx)
- 0x30002: ERROR_FILE_NOT_FOUND
- 0x30011: ERROR_FILE_EXISTS
- 0x30014: ERROR_IS_DIRECTORY
- 0x30015: ERROR_NOT_DIRECTORY
- 0x3001e: ERROR_READ_ONLY_FS
- 0x3005b: ERROR_NAME_TOO_LONG

### Network Errors (0x4xxxx)
- 0x40062: ERROR_CONNECTION_TIMEOUT
- 0x40067: ERROR_CONNECTION_REFUSED
- 0x4006a: ERROR_ADDRESS_IN_USE
- 0x4006e: ERROR_SOCKET_CLOSED
- 0x40071: ERROR_INVALID_PACKET_MAGIC
- 0x40072: ERROR_SEQUENCE_ERROR
- 0x40073: ERROR_INVALID_HANDSHAKE
- 0x40074: ERROR_PACKET_TOO_LARGE
- 0x40075: ERROR_SOCKET_ALREADY_CLOSED
- 0x40077: ERROR_CONNECTION_RESET
- 0x40078: ERROR_CONNECTION_ABORTED
- 0x40082: ERROR_NETWORK_UNREACHABLE

### Special Errors
- 0x40001: ERROR_INVALID_MAGIC (struct magic validation)
- 0x00000: ERROR_NONE (no error)
- 0xfffff: ERROR_UNKNOWN (catch-all)

### errno Mapping
- Many errno values map to one KDX error (error coalescing)
- Implementation: `hx_errno_to_error()` in libhaxial
- Examples:
  - EPIPE, ENOTCONN, ESHUTDOWN → ERROR_SOCKET_CLOSED
  - ECONNRESET, ENETRESET → ERROR_CONNECTION_RESET
  - ECONNREFUSED → ERROR_CONNECTION_REFUSED

---

## Network Constants

### Ports (Suspected)
- **Main port**: 10700 (0x29cc) - seen in ServerState +0x258
- **Tracker port**: 11177 (0x2ba9) - seen in ServerState +0x278

### Limits
- **Max packet size**: 2048 bytes (0x800)
- **Max buffer**: 200KB (0x32000) - ServerState data_buffer
- **Large buffer**: 1MB (0x100000) - ServerState +0x2c8

---

## Data Types

### Pascal Strings
Format: `[uint8 length][char data[length]]`
- No null terminator
- Max length: 255 bytes
- Used extensively throughout protocol

### Random Number Generation
- 256-byte pool with FNV-1a mixing
- LCG seed: constants 0x41c64e6d, 0x3039
- Microsecond-seeded entropy
- Implementation: `hx_random()` in libhaxial

---

## Crypto

### SHA-1
- Used for: Unknown (authentication? integrity?)
- Implementation: `libhaxial/src/crypto/sha1.c`

### Blowfish
- Used for: Unknown (session encryption?)
- Implementation: `libhaxial/src/crypto/blowfish.c`

### Handshake Cipher
- Constant 0x4878 used in handshake packet construction
- Purpose: Unknown (key exchange? challenge?)

---

## Protocol Flow (High-Level)

```
1. Client connects to server port 10700
2. Server creates ConnectionState (magic 0x2aa24aec)
3. Server sends handshake packet (uses cipher 0x4878)
4. Client responds (format unknown)
5. Server validates → handshake_state = AUTHENTICATED
6. Normal packet exchange begins (PXTP magic)
7. Message/command processing (not yet analyzed)
```

---

## Unknowns (Needs Analysis)

### Critical Unknowns
- Complete packet header format
- Message/command structure (how different message types are identified)
- Packet payload format
- Authentication mechanism (password format?)
- Session encryption (when/how Blowfish is used?)
- File transfer protocol
- Chat message format
- News item format

### Minor Unknowns
- Exact purpose of SHA-1 in protocol
- Complete handshake packet structure
- Sequence number algorithm
- Tracker protocol (if different from main)

---

## References
- Binary: KDXServer.lexe
- Analysis: Ghidra + AGENTS.md workflow
- Implementation: libhaxial (C23)
