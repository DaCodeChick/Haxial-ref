# Haxial KDX - C/C++ Implementation

A clean-room, binary-compatible reimplementation of Haxial KDX client/server/tracker system through reverse engineering.

## Project Overview

Haxial KDX is a vintage client/server communication platform with features including:
- Multi-user chat (public and private)
- File sharing and transfer
- News/message boards
- User management and permissions
- Tracker system for server discovery

This project aims to create a modern, cross-platform implementation by reverse-engineering the original KDX protocol from binaries.

## Architecture

### Components

- **libhaxial** (LGPL-3.0): Core protocol library in C
  - Pascal string implementation
  - Packet encoding/decoding
  - Connection management
  - Transaction handling
  - Cross-platform socket abstraction

- **haxial-server** (GPL-3.0): Server daemon in C
  - Multi-client connection handling
  - User authentication
  - File management
  - Chat and news systems

- **haxial-tracker** (GPL-3.0): Tracker service in C
  - Server registration and discovery
  - Server list management
  - Heartbeat monitoring

- **haxial-client** (GPL-3.0): GUI client in C++23 + Qt 6
  - Modern Qt 6 interface
  - Server browser
  - Chat interface
  - File transfer UI
  - News reader

## Platform Support

- **Linux**: glibc-based distributions (POSIX sockets, pthread)
- **macOS**: 11.0+ (Intel and Apple Silicon)
- **Windows**: Windows 10+ with UCRT (Winsock2, Win32 threads)

## Building

### Prerequisites

- CMake 3.20+
- C11 compiler (GCC, Clang, or MSVC)
- C++23 compiler (GCC 13+, Clang 16+, or MSVC 2022)
- Qt 6.2+ (for client only)
- Ninja (recommended) or Make

### Quick Start

```bash
# Configure (Linux)
cmake --preset linux-glibc-debug

# Build
cmake --build build/linux-glibc-debug

# Run server
./build/linux-glibc-debug/server/haxial-server

# Run client
./build/linux-glibc-debug/client/haxial-client
```

### CMake Presets

Available presets:
- `linux-glibc-debug` / `linux-glibc-release`
- `macos-debug` / `macos-release`
- `windows-ucrt-debug` / `windows-ucrt-release`

## Protocol Reverse Engineering

The KDX protocol is being reverse-engineered using Ghidra. See:

- **[AGENTS.md](AGENTS.md)**: Ghidra MCP analysis guidelines
- **[docs/protocol/](docs/protocol/)**: Protocol documentation
- **[docs/protocol/findings/](docs/protocol/findings/)**: Per-binary analysis notes

### Key Protocol Features

- **Pascal Strings**: 1-byte length prefix + data (no null terminator)
- **Binary Packets**: Fixed header + TLV parameter blocks
- **Transaction-Based**: Request/response model with transaction IDs
- **TCP-Based**: No encryption in original protocol

## Development Status

🚧 **Early Development** - Protocol analysis in progress

- [x] Project structure and build system
- [x] Documentation framework
- [ ] Protocol analysis (KDXServer.lexe, KDX Client)
- [ ] libhaxial implementation
- [ ] Server implementation
- [ ] Tracker implementation
- [ ] Client implementation

## License

- **libhaxial**: LGPL-3.0-or-later (allows use in proprietary software)
- **Server/Tracker/Client**: GPL-3.0-or-later

See [LICENSE-LGPL-3.0](LICENSE-LGPL-3.0) and [LICENSE-GPL-3.0](LICENSE-GPL-3.0).

## Contributing

This is a reverse engineering project focused on protocol compatibility. Contributions welcome for:

- Binary analysis with Ghidra
- Protocol documentation
- Implementation of documented features
- Testing with original KDX binaries
- Cross-platform compatibility

## Disclaimer

This is an independent reimplementation through reverse engineering for interoperability and preservation purposes. Not affiliated with original Haxial KDX developers.

## Resources

- Original software: Haxial KDX (vintage Mac/Windows)
- Analysis tool: Ghidra 11.0+
- Build system: CMake 3.20+
- UI framework: Qt 6

---

**Current Focus**: Analyzing KDXServer.lexe to document packet format and protocol constants.
