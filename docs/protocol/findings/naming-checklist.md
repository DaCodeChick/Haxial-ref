# Naming Checklist - ZERO TOLERANCE

## Rule: NO UNNAMED VARIABLES ALLOWED

When you analyze ANY function, you MUST rename:
- ✗ param_1, param_2, param_3... → ✓ connection, offset, buffer, length
- ✗ local_*, iVar*, uVar*, pvVar* → ✓ skip_bytes, remaining, chunk_size
- ✗ int*, uint* (for structures) → ✓ ConnectionState*, SocketInfo*

## Functions Completed

### bind_and_listen ✓
- ✓ socket_struct: SocketInfo*
- ✓ port: uint
- ✓ enable_reuse: local var
- ✓ addr: local var

### socket_recv_packet ✓
- ✓ socket_struct: SocketInfo*
- ✓ out_bytes_read: int*
- ✓ out_more_data: byte*
- ⚠ LOCALS NOT FULLY RENAMED

### socket_send_packet ✓
- ✓ socket_struct: SocketInfo*
- ✓ data: void*
- ✓ length: size_t
- ⚠ LOCALS NOT FULLY RENAMED

### connection_receive_packets ✓
- ✓ connection: ConnectionState*
- ⚠ LOCALS NOT FULLY RENAMED

### handle_connection_handshake ⚠ PARTIAL
- ✓ connection: ConnectionState*
- ✓ cipher_key: local var
- ✓ cipher_buffer: local var
- ✓ bytes_received: local var
- ✗ MANY locals still unnamed (local_86c, local_868, local_864, etc.)
- ✗ MANY uVar*, iVar* still unnamed

### connection_read_buffer ✓
- ✓ connection: int (should be ConnectionState*)
- ✓ offset: uint
- ✓ out_buffer: void*
- ✓ length: uint
- ✓ skip_bytes: local var
- ✓ bytes_to_copy: local var
- ✓ remaining: local var

## Functions Still Completely Unnamed

- connection_consume_bytes
- connection_send_handshake_response
- connection_send_handshake_packet
- log_error
- log_socket_error
- memset_zero
- lock_acquire
- lock_release
- FUN_08071bec
- FUN_0808f6cc
- FUN_0808f7ec
- FUN_08098e44
- FUN_08098d3c
- FUN_0806edd8
- FUN_08098ea0
- FUN_0808fcfc
- ... and many more

## Next Steps

1. Go back to handle_connection_handshake and rename ALL remaining locals
2. For each helper function called:
   - Rename ALL parameters
   - Rename ALL locals
   - Apply struct types
3. Continue recursively for every function touched

