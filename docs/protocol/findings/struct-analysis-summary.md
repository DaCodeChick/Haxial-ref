# Structure Analysis Summary

## Structures Created

### 1. BlowfishContext (crypto)
**Size:** 4168 bytes  
**Location:** /crypto/BlowfishContext

```c
struct BlowfishContext {
    uint s_box_0[256];      // offset 0x0000 - S-box 0
    uint s_box_1[256];      // offset 0x0400 - S-box 1
    uint s_box_2[256];      // offset 0x0800 - S-box 2
    uint s_box_3[256];      // offset 0x0C00 - S-box 3
    uint p_array[18];       // offset 0x1000 - P-array (P0-P17)
};
```

**Usage:**
- `blowfish_encrypt(BlowfishContext *ctx, uint *input_block, byte *output_block)`
- Used in connection handshake for encryption

### 2. SHA1Context (crypto)
**Size:** 92 bytes  
**Location:** /crypto/SHA1Context

```c
struct SHA1Context {
    uint state[5];       // H0-H4 (SHA-1 state)
    uint count[2];       // message length in bits
    byte buffer[64];     // input buffer
};
```

**Usage:**
- `sha1_hash(byte *data, uint length, uint *digest)`
- Used for password hashing and handshake authentication

### 3. SocketInfo (network)
**Size:** ~92+ bytes (still discovering fields)  
**Location:** /

```c
struct SocketInfo {
    uint magic;                // +0x00 (0x42781ea5)
    uint lock_or_refcount;     // +0x04
    void *event_context;       // +0x08 (param_1[1] in some functions)
    byte gap_0x0c[8];
    uint socket_fd;            // +0x10
    uint error_code;           // +0x14
    byte gap_0x18[20];
    void *send_queue_head;     // +0x24 (param_1[9])
    byte gap_0x28[28];
    uint bytes_sent;           // +0x44 (param_1[0x11])
    byte gap_0x48[16];
    byte socket_state;         // +0x58
    byte is_udp;               // +0x59
    byte gap_0x5a[?];
};
```

**Fields Discovered:**
- `magic`: Socket magic number (0x42781ea5)
- `socket_fd`: File descriptor for the socket
- `error_code`: Last error code
- `socket_state`: State byte (0x1c = closed/shutdown)
- `is_udp`: Boolean flag for UDP vs TCP
- `send_queue_head`: Pointer to send queue
- `bytes_sent`: Number of bytes sent
- `event_context`: Context for event system

**Usage:**
- `socket_shutdown(SocketInfo *socket)`
- `socket_set_options(SocketInfo *socket)`
- `socket_send_or_queue(SocketInfo *socket, void **buffer_ref, uint length)`
- `create_socket(...) -> SocketInfo*`
- `bind_and_listen(SocketInfo *socket, ...)`

### 4. ConnectionState (network)
**Size:** Unknown (large structure)  
**Location:** /

```c
struct ConnectionState {
    uint magic;            // +0x00 (0x2aa24aec)
    // ... many fields TBD
};
```

**Usage:**
- `handle_connection_handshake(ConnectionState *connection)`
- `connection_consume_bytes(ConnectionState *connection, uint bytes)`
- `connection_read_buffer(ConnectionState *connection, uint offset, void *buffer, uint length)`
- `connection_send_handshake_response(ConnectionState *connection)`
- `connection_send_handshake_packet(ConnectionState *connection, char packet_type)`
- `connection_receive_packets(ConnectionState *connection)`

**Still TODO:** Map out all ConnectionState fields

## Functions with Correct Types ✓

All these functions now have properly typed struct parameters:

1. **blowfish_encrypt** - BlowfishContext*, uint*, byte*
2. **sha1_hash** - byte*, uint, uint*
3. **socket_shutdown** - SocketInfo*
4. **socket_set_options** - SocketInfo*
5. **socket_send_or_queue** - SocketInfo*, void**, uint
6. **create_socket** - returns SocketInfo*
7. **bind_and_listen** - SocketInfo*, ...
8. **handle_connection_handshake** - ConnectionState*
9. **connection_consume_bytes** - ConnectionState*, uint
10. **connection_read_buffer** - ConnectionState*, uint, void*, uint
11. **connection_send_handshake_response** - ConnectionState*
12. **connection_send_handshake_packet** - ConnectionState*, char
13. **connection_receive_packets** - ConnectionState*

## Protocol Constants Discovered

### Magic Numbers
- **Socket magic**: `0x42781ea5`
- **Connection magic**: `0x2aa24aec`

### Socket States
- **0x1c**: Closed/shutdown state

### Error Codes
- **0x4006e**: Socket closed error

### Network Configuration
- **Default server port**: 10700
- **Default tracker port**: 10800
- **Listen backlog**: 1024
- **Buffer size**: 0x20000 (128KB) for SO_SNDBUF/SO_RCVBUF

### Handshake Constants
- **Cipher constant**: 0x4878 (used in handshake cipher logic)

## Analysis Workflow Used

For each function analyzed:
1. Get decompiled code
2. Identify struct access patterns (offset calculations)
3. Create or update struct definition with ghidra_struct
4. Rename ALL variables (zero tolerance policy)
5. Set prototype with correct types using ghidra_variables
6. Verify by re-decompiling
7. Document findings

## Next Steps

1. **Complete SocketInfo mapping**: Find all remaining fields
2. **Complete ConnectionState mapping**: Fully map this critical structure
3. **Analyze send queue structures**: Understand queue entry layout
4. **Analyze event system structures**: Understand fd_set management
5. **Continue recursive renaming**: Keep applying zero-tolerance policy to all called functions

