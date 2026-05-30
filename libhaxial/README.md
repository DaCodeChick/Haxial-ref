# libhaxial

Binary-compatible reimplementation of the Haxial KDX protocol library.

## Overview

libhaxial is a clean-room reimplementation of the network protocol used by Haxial KDX (Knowledge Database eXchange), a classic Mac OS file sharing and collaboration server/client from the early 2000s. This library aims to provide a modern, cross-platform implementation that maintains binary compatibility with the original KDX protocol.

## Status

**Early Development** - Currently implementing core cryptographic primitives and protocol foundations.

### Implemented
- ✅ Error code system (from binary analysis)
- ✅ SHA-1 hash implementation (RFC 3174 compliant)
- ✅ Blowfish encryption (ECB mode)
- ✅ Basic test suite

### In Progress
- 🔄 Socket abstraction layer
- 🔄 Connection state management
- 🔄 Packet framing and serialization

### Planned
- ⏳ Transaction handlers
- ⏳ Authentication system
- ⏳ File transfer protocol
- ⏳ Chat protocol
- ⏳ Tracker protocol

## Building

### Requirements
- CMake 3.15 or later
- C23-compatible compiler (GCC 13+, Clang 16+)
- Linux/macOS/Windows

### Build Instructions

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run tests
ctest --test-dir build

# Install (optional)
cmake --install build --prefix /usr/local
```

### Build Options

- `BUILD_SHARED_LIBS` - Build as shared library (default: ON)
- `BUILD_TESTS` - Build test suite (default: ON)

## Usage

```c
#include <haxial.h>

// Example: SHA-1 hash
uint8_t digest[20];
const char *data = "Hello, KDX!";
hx_sha1_hash((const uint8_t *)data, strlen(data), digest);

// Example: Blowfish encryption
TBlowfishContext ctx;
uint8_t key[] = "secret";
uint8_t plaintext[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint8_t ciphertext[8];

hx_blowfish_init(&ctx, key, sizeof(key) - 1);
hx_blowfish_encrypt(&ctx, plaintext, ciphertext);
```

## Naming Conventions

libhaxial follows consistent naming conventions:

- **Functions**: `hx_` prefix, snake_case (e.g., `hx_sha1_init()`)
- **Types**: `T` prefix, CamelCase (e.g., `TBlowfishContext`)
- **Constants**: SCREAMING_SNAKE_CASE (e.g., `HX_ERROR_SUCCESS`)
- **Internal functions**: `hxi_` prefix (not exported)

## Reverse Engineering

This library is based on reverse engineering of the original KDX binaries using Ghidra. All protocol details, constants, and algorithms are extracted through static analysis without access to original source code.

See [docs/protocol/](../docs/protocol/) for detailed analysis notes and findings.

## License

- **libhaxial library**: LGPL-3.0-or-later
- **KDX client/server/tracker**: GPL-3.0-or-later (when implemented)

## Project Structure

```
libhaxial/
├── include/haxial/     # Public API headers
│   ├── errors.h        # Error codes
│   ├── sha1.h          # SHA-1 implementation
│   ├── blowfish.h      # Blowfish cipher
│   └── ...
├── src/                # Implementation
│   ├── crypto/         # Cryptographic primitives
│   └── net/            # Network layer
├── tests/              # Test suite
└── CMakeLists.txt      # Build configuration
```

## Contributing

This is a reverse engineering and compatibility project. Contributions should:

1. Be based on analysis of original binaries (not speculation)
2. Maintain binary compatibility with original protocol
3. Follow the established naming conventions
4. Include tests for new functionality
5. Document findings in `docs/protocol/findings/`

## References

- [Haxial KDX History](https://en.wikipedia.org/wiki/KDX_(software))
- Original software: Haxial KDX Server/Client (2000-2004)
- Analysis target: KDXServer.lexe (Linux x86 binary)

## Acknowledgments

- Haxial Computing Inc. for creating the original KDX software
- Ghidra reverse engineering framework
- The classic Mac and Linux server community

---

**Note**: This is an independent reimplementation project for compatibility and preservation purposes. Not affiliated with or endorsed by Haxial Computing Inc.
