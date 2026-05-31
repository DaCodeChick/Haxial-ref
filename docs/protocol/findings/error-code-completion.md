# KDXErrorCode Enum Completion

**Date**: 2026-05-30  
**Issue**: KDXErrorCode enum was incomplete - only had 5 values instead of 27

## Problem

The original enum only contained:
- ERROR_INVALID_PARAM (0x10002)
- ERROR_INVALID_MAGIC (0x10019)
- ERROR_OUT_OF_MEMORY (0x20065)
- ERROR_SOCKET_ALREADY_CLOSED (0x40066)
- ERROR_SOCKET_CLOSED (0x4006e)

But analysis of `errno_to_kdx_error` and `validate_packet_header` revealed **22 additional error codes**.

## Solution

Created complete enum with all 27 error codes discovered from binary analysis.

## Complete KDXErrorCode Enum

### System/General Errors (0x1xxxx)
| Code | Name | Source | Notes |
|------|------|--------|-------|
| 0x10002 | ERROR_INVALID_PARAM | errno 22 (EINVAL) | Invalid parameter |
| 0x1000e | ERROR_ARGUMENT_TOO_LONG | errno 7 (E2BIG) | Argument list too long |
| 0x10012 | ERROR_NO_SUCH_PROCESS | errno 3 (ESRCH) | No such process |
| 0x10014 | ERROR_PERMISSION_DENIED | errno 1,13 (EPERM/EACCES) | Permission denied |
| 0x10016 | ERROR_RESOURCE_BUSY | errno 16 (EBUSY) | Resource busy |
| 0x10019 | ERROR_INVALID_MAGIC | application | Invalid magic number (struct) |
| 0x1001b | ERROR_UNKNOWN | errno default | Unknown error |

### Memory/Address Errors (0x2xxxx)
| Code | Name | Source | Notes |
|------|------|--------|-------|
| 0x20065 | ERROR_OUT_OF_MEMORY | errno 12 (ENOMEM) | Out of memory |
| 0x20066 | ERROR_BAD_ADDRESS | errno 14 (EFAULT) | Bad memory address |

### File System Errors (0x3xxxx)
| Code | Name | Source | Notes |
|------|------|--------|-------|
| 0x30066 | ERROR_FILE_NOT_FOUND | errno 2 (ENOENT) | File not found |
| 0x3006a | ERROR_IS_DIRECTORY | errno 21 (EISDIR) | Is a directory |
| 0x3006b | ERROR_NOT_DIRECTORY | errno 20 (ENOTDIR) | Not a directory |
| 0x3006e | ERROR_READ_ONLY_FS | errno 30 (EROFS) | Read-only filesystem |
| 0x30073 | ERROR_FILE_EXISTS | errno 17 (EEXIST) | File exists |
| 0x30076 | ERROR_NAME_TOO_LONG | errno 36 (ENAMETOOLONG) | Filename too long |

### Network/Socket Errors (0x4xxxx)
| Code | Name | Source | Notes |
|------|------|--------|-------|
| 0x40066 | ERROR_SOCKET_ALREADY_CLOSED | application | Socket already closed |
| 0x40069 | ERROR_ADDRESS_IN_USE | errno 98,99 | Address in use/unavailable |
| 0x4006a | ERROR_CONNECTION_RESET | errno 104 (ECONNRESET) | Connection reset |
| 0x4006b | ERROR_CONNECTION_REFUSED | errno 111 (ECONNREFUSED) | Connection refused |
| 0x4006c | ERROR_CONNECTION_TIMEOUT | errno 110 (ETIMEDOUT) | Connection timeout |
| 0x4006d | ERROR_CONNECTION_ABORTED | errno 102,103,112 | Connection aborted |
| 0x4006e | ERROR_SOCKET_CLOSED | application | Socket closed |
| 0x4006f | ERROR_NETWORK_UNREACHABLE | errno 100,101,113 | Network unreachable |

### Protocol/Packet Errors (0x4xxxx high)
| Code | Name | Source | Notes |
|------|------|--------|-------|
| 0x40071 | ERROR_INVALID_PACKET_MAGIC | validate_packet_header | Wrong packet magic "PXTP" |
| 0x40072 | ERROR_SEQUENCE_ERROR | validate_packet_header | Sequence number mismatch |
| 0x40073 | ERROR_INVALID_HANDSHAKE | validate_packet_header | Invalid handshake state |
| 0x40075 | ERROR_PACKET_TOO_LARGE | validate_packet_header | Packet exceeds 2048 bytes |

## Error Code Structure

KDX uses a structured error code scheme:

```
0xTTTTEE
  ||  ||
  ||  ++-- Specific error within category (EE)
  ++------ Error category/type (TTTT)
```

### Categories:
- **0x1xxxx**: System-level errors (permissions, processes, parameters)
- **0x2xxxx**: Memory and address errors
- **0x3xxxx**: File system operations
- **0x4xxxx**: Network, sockets, and protocol errors

## Many-to-One errno Mappings

Multiple POSIX errno values map to single KDX errors for simplification:

- **Permission errors → 0x10014**: EPERM(1), EACCES(13)
- **Address errors → 0x40069**: EADDRINUSE(98), EADDRNOTAVAIL(99)
- **Network unreachable → 0x4006f**: ENETDOWN(100), ENETUNREACH(101), EHOSTUNREACH(113)
- **Connection aborted → 0x4006d**: ENETRESET(102), ECONNABORTED(103), EHOSTDOWN(112)

This is **intentional error coalescing**, not ambiguity - simplifies error handling for applications.

## Files Updated

1. **Ghidra**: KDXErrorCode enum expanded from 5 → 27 values
2. **libhaxial/include/haxial/errors.h**: Corrected all error codes to match binary
3. **docs/protocol/findings/errno-mapping-analysis.md**: Complete errno→KDX mapping

## Impact

With complete error codes:
- ✅ Decompilation shows meaningful error names
- ✅ libhaxial matches actual protocol
- ✅ Error handling can be properly implemented
- ✅ Protocol documentation is accurate
- ✅ All 27 error codes now documented

## Status

✅ **COMPLETE** - All error codes from binary analysis extracted and documented
