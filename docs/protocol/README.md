# Haxial KDX Protocol Documentation

## Overview

This directory contains comprehensive documentation of the Haxial KDX network protocol, reverse-engineered from the original binaries.

## Protocol Architecture

Haxial KDX uses a client/server architecture with an optional tracker for server discovery:

- **Client ↔ Server**: Primary connection for chat, file transfer, user management
- **Server ↔ Tracker**: Server registration and discovery
- **Client ↔ Tracker**: Server list queries

All connections use TCP with custom binary protocol over the socket layer.

## Connection Lifecycle

1. **Connection Establishment**: TCP connection to server (default port: TBD)
2. **Handshake**: Protocol version negotiation and capability exchange
3. **Authentication**: User login with credentials
4. **Session**: Authenticated communication using transactions
5. **Disconnect**: Clean shutdown or timeout

## Protocol Layers

### Transport Layer
- TCP/IP sockets
- No encryption in original protocol (plaintext)
- Network byte order (big-endian) for multi-byte integers

### Packet Layer
- Fixed-size header with variable-length payload
- Magic number for protocol identification
- Transaction-based request/response model
- Parameter blocks (Type-Length-Value format)

### Application Layer
- Transaction types (opcodes) for different operations
- User authentication and session management
- Chat messages (public and private)
- File operations (list, download, upload)
- News reading and posting
- User management

## Documentation Structure

- **[connection.md](connection.md)** - Connection establishment and handshake
- **[packets.md](packets.md)** - Packet format and structure
- **[transactions.md](transactions.md)** - Transaction types and parameters
- **[data-types.md](data-types.md)** - Data type specifications (Pascal strings, etc.)
- **[constants.md](constants.md)** - Protocol constants, magic numbers, opcodes
- **[findings/](findings/)** - Per-binary analysis notes from Ghidra

## Reverse Engineering Status

Analysis status for each binary:

| Binary | Status | Progress |
|--------|--------|----------|
| KDX Server (Mac) | Not Started | 0% |
| KDX Client (Mac) | Not Started | 0% |
| KDX Client (Windows) | Not Started | 0% |
| KDX Tracker | Not Started | 0% |

See [findings/analysis-status.md](findings/analysis-status.md) for detailed progress.

## Binary Compatibility Goals

This documentation aims to achieve **binary-compatible** reimplementation:

- Exact packet format matching
- Compatible transaction opcodes
- Same parameter types and encoding
- Compatible with original clients and servers

## Contributing to Documentation

When analyzing binaries with Ghidra:

1. Follow guidelines in [AGENTS.md](../../AGENTS.md)
2. Document findings in `findings/` directory
3. Update relevant protocol documentation
4. Cross-reference with other binaries
5. Mark uncertainties with `[UNCERTAIN]` tag

## References

- AGENTS.md: Ghidra analysis guidelines
- findings/: Detailed binary analysis notes

## Glossary

- **Transaction**: A request/response exchange between client and server
- **Parameter Block**: TLV-encoded data in packet payload
- **Pascal String**: 1-byte length prefix + string data (no null terminator)
- **Opcode**: Operation code identifying transaction type
- **Magic Number**: Protocol signature in packet header

---

**Last Updated**: 2026-05-29  
**Status**: Initial documentation framework - awaiting binary analysis
