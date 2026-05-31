# KDX Protocol Analysis - Current Status

**Last Updated**: 2026-05-30  
**Binary**: KDXServer.lexe  
**Analysis Method**: Ghidra MCP + AGENTS.md workflow

## Overview

Reverse engineering the KDX protocol from Haxial KDX Server binary to create binary-compatible reimplementation.

## Completed

### libhaxial Library ✅
- SHA-1 implementation (opaque struct)
- Blowfish encryption (opaque struct)
- Intrusive linked lists (TListNode)
- FNV-1a hash (32-bit)
- Pascal string helpers
- Random number generation (matches binary exactly)
- errno → KDX error code conversion
- Complete error code catalog (27 codes)
- All tests passing

### Error Codes ✅
- Complete KDXErrorCode enum (27 values)
- Structure: 0x1xxxx=system, 0x2xxxx=memory, 0x3xxxx=filesystem, 0x4xxxx=network
- errno conversion function implemented and tested
- Many-to-one mappings documented

### Structs Identified ✅
- **SocketInfo**: 256 bytes, 10 fields named (magic, socket_fd, error_code, etc.)
- **ConnectionState**: ~746 bytes, 8 fields identified (handshake_state, socket, callbacks)
- **ServerState**: 746 bytes, 10 fields named (sockets, databases, folders, buffers)

### Functions Analyzed (AGENTS.md compliant) ✅
- binary_search: Complete with all params/locals renamed
- socket_shutdown: Complete
- socket_send_or_queue: Complete, named struct fields
- handle_send_error: Complete
- socket_flush_send_queue: Complete
- validate_packet_header: Complete, protocol discovery
- errno_to_kdx_error: Complete
- initialize_send_queue_pool: Complete
- event_set_fd_bit: Complete
- lock_release: Complete

### Protocol Discoveries ✅
- **Packet magic**: "PXTP" (0x50545850)
- **Handshake states**: 1=initial, 3=retry, 4=authenticated, 8=failed
- **Error codes**: 0x40071-0x40075 for protocol errors
- **Max packet size**: 2048 bytes (0x800)

## In Progress

### Struct Field Mapping
- SocketInfo: ~15 unnamed fields remain
- ConnectionState: Most fields unnamed
- ServerState: Many unnamed fields

### Function Analysis
- server_initialize: Analyzed but needs variable renaming
- server_event_loop: Not yet analyzed
- Connection handlers: Partially analyzed
- Transaction handlers: Not yet started

## Next Steps

1. **Continue struct field identification**
   - Analyze more functions that use SocketInfo/ConnectionState/ServerState
   - Name fields only when understood (AGENTS.md policy)

2. **Transaction handlers**
   - Find transaction dispatch table
   - Identify all transaction types
   - Document transaction format

3. **Packet structure**
   - Complete packet header format
   - Parameter block structure (TLV format suspected)
   - Document all packet types

4. **Protocol flow**
   - Complete handshake sequence
   - Authentication mechanism
   - Session management

## Files Structure

- `STRUCTS.md` - Struct definitions and field mappings
- `PROTOCOL.md` - Packet format, magic numbers, handshake
- `FUNCTIONS.md` - Key functions analyzed
- `UNKNOWNS.md` - Things we don't understand yet

## Naming Conventions (libhaxial)

- Functions: `hx_` prefix, snake_case
- Types: `T` prefix, CamelCase
- Constants: SCREAMING_SNAKE_CASE
- Error codes: `HX_ERROR_*`
