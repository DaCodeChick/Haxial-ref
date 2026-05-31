# errno to KDX Error Code Mapping

**Function**: `errno_to_kdx_error` (0x08091820)  
**Purpose**: Converts POSIX errno values to KDX application error codes

## Mapping Table

| errno | errno name | KDX Error Code | Notes |
|-------|------------|----------------|-------|
| 1 | EPERM | 0x10014 | Permission denied |
| 2 | ENOENT | 0x30066 | File/entry not found |
| 3 | ESRCH | 0x10012 | No such process |
| 7 | E2BIG | 0x1000e | Argument list too long |
| 12 | ENOMEM | ERROR_OUT_OF_MEMORY | Out of memory |
| 13 | EACCES | 0x10014 | Permission denied (same as EPERM) |
| 14 | EFAULT | 0x20066 | Bad address |
| 16 | EBUSY | 0x10016 | Device or resource busy |
| 17 | EEXIST | 0x30073 | File exists |
| 20 | ENOTDIR | 0x3006b | Not a directory |
| 21 | EISDIR | 0x3006a | Is a directory |
| 22 | EINVAL | ERROR_INVALID_PARAM | Invalid argument |
| 30 | EROFS | 0x3006e | Read-only file system |
| 36 | ENAMETOOLONG | 0x30076 | File name too long |
| 98 | EADDRINUSE | 0x40069 | Address already in use |
| 99 | EADDRNOTAVAIL | 0x40069 | Address not available (same as EADDRINUSE) |
| 100 | ENETDOWN | 0x4006f | Network is down |
| 101 | ENETUNREACH | 0x4006f | Network unreachable (same as ENETDOWN) |
| 102 | ENETRESET | 0x4006d | Network connection reset |
| 103 | ECONNABORTED | 0x4006d | Connection aborted (same as ENETRESET) |
| 104 | ECONNRESET | 0x4006a | Connection reset by peer |
| 110 | ETIMEDOUT | 0x4006c | Connection timed out |
| 111 | ECONNREFUSED | 0x4006b | Connection refused |
| 112 | EHOSTDOWN | 0x4006d | Host is down (same as ENETRESET) |
| 113 | EHOSTUNREACH | 0x4006f | No route to host (same as ENETDOWN) |
| default | (any other) | 0x1001b | Unknown/generic error |

## Many-to-One Mappings

Multiple errno values map to the same KDX error in these cases:

### Permission Errors → 0x10014
- EPERM (1)
- EACCES (13)

**Rationale**: Both indicate permission/access denied from application perspective.

### Address Errors → 0x40069
- EADDRINUSE (98)
- EADDRNOTAVAIL (99)

**Rationale**: Both indicate socket address binding issues.

### Network Unreachable → 0x4006f
- ENETDOWN (100)
- ENETUNREACH (101)
- EHOSTUNREACH (113)

**Rationale**: All three indicate "cannot reach destination" - network layer, routing, or host unreachable.

### Connection Aborted → 0x4006d
- ENETRESET (102)
- ECONNABORTED (103)
- EHOSTDOWN (112)

**Rationale**: All indicate connection was terminated abnormally by network or remote host.

## KDX Error Code Ranges

From the mappings, we can infer KDX error code structure:

- **0x1xxxx**: System/general errors (memory, permissions, process)
- **0x2xxxx**: Memory/address errors
- **0x3xxxx**: File system errors
- **0x4xxxx**: Network/socket errors

## Analysis Notes

1. **This is NOT ambiguous** - it's intentional error code consolidation
2. Multiple similar errno values → single application error = correct design
3. Application doesn't need to distinguish between "network down" vs "host unreachable"
4. Simplifies error handling for KDX protocol clients
5. The function properly handles all common POSIX network errors

## Usage

This function is called by:
- `throw_errno_exception` (0x080918c4) - converts errno and throws C++ exception
- Socket error handlers - converts system errors to protocol errors
- File operation wrappers - converts file system errors

## Related Functions

- `throw_errno_exception` - wraps this function and throws exception
- `handle_send_error` - stores raw errno in `socket->last_errno` before conversion
- `log_error` - logs KDX error codes with source file and line number

## Conclusion

The "ambiguity" is intentional error coalescing - this is a **correct** design pattern for error code translation. The function properly categorizes similar system errors into meaningful application-level error codes.
