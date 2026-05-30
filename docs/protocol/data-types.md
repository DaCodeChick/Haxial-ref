# KDX Data Types

## Overview

This document specifies the data types used in the KDX protocol for encoding parameters and data payloads.

## Primitive Types

### Integers

All integers use **network byte order (big-endian)**.

| Type     | Size | Range                    | Usage                        |
|----------|------|--------------------------|------------------------------|
| uint8    | 1    | 0 to 255                 | Flags, small counts          |
| uint16   | 2    | 0 to 65,535              | IDs, opcodes, small sizes    |
| uint32   | 4    | 0 to 4,294,967,295       | Sizes, timestamps, large IDs |
| int16    | 2    | -32,768 to 32,767        | Signed values                |
| int32    | 4    | -2,147,483,648 to ...    | Signed values                |

**Endianness Conversion**:
```c
#include <arpa/inet.h>  // POSIX
// or <winsock2.h> on Windows

uint16_t net16 = htons(host16);  // Host to network (16-bit)
uint32_t net32 = htonl(host32);  // Host to network (32-bit)

uint16_t host16 = ntohs(net16);  // Network to host (16-bit)
uint32_t host32 = ntohl(net32);  // Network to host (32-bit)
```

## Pascal String (pstring)

**CRITICAL DATA TYPE**: KDX uses Pascal strings extensively.

### Structure

```
[uint8_t length][char data[length]]
```

- **1 byte**: String length (0-255)
- **N bytes**: String data (ASCII or UTF-8)
- **No null terminator**

### Maximum Length

255 characters (limited by 1-byte length field)

### Encoding

```
Offset | Size | Type      | Field    | Description
-------|------|-----------|----------|----------------------------
0x00   | 1    | uint8     | length   | Number of characters
0x01   | N    | char[]    | data     | Character data (no null)
```

### Example

String "Hello":
```
Hex:  05 48 65 6C 6C 6F
      ^  ^--------------^
      |  Character data: "Hello"
      Length: 5
```

Empty string:
```
Hex:  00
      ^
      Length: 0 (no data follows)
```

### C Implementation

```c
typedef struct {
    uint8_t length;
    char data[255];  // Maximum capacity
} hx_pstring_t;

// Create from C string
void pstring_from_cstring(hx_pstring_t* pstr, const char* cstr) {
    size_t len = strlen(cstr);
    if (len > 255) len = 255;  // Truncate if too long
    pstr->length = (uint8_t)len;
    memcpy(pstr->data, cstr, len);
}

// Convert to C string (allocates)
char* pstring_to_cstring(const hx_pstring_t* pstr) {
    char* cstr = malloc(pstr->length + 1);
    memcpy(cstr, pstr->data, pstr->length);
    cstr[pstr->length] = '\0';  // Add null terminator
    return cstr;
}

// Serialize to buffer
size_t pstring_serialize(uint8_t* buf, const hx_pstring_t* pstr) {
    buf[0] = pstr->length;
    memcpy(buf + 1, pstr->data, pstr->length);
    return 1 + pstr->length;
}

// Deserialize from buffer
size_t pstring_deserialize(hx_pstring_t* pstr, const uint8_t* buf) {
    pstr->length = buf[0];
    memcpy(pstr->data, buf + 1, pstr->length);
    return 1 + pstr->length;
}
```

### Comparison

Pascal strings can be compared by:
1. First comparing lengths
2. If equal, comparing data bytes

```c
int pstring_compare(const hx_pstring_t* a, const hx_pstring_t* b) {
    if (a->length != b->length) {
        return a->length - b->length;
    }
    return memcmp(a->data, b->data, a->length);
}
```

### Common Uses

- Usernames
- Passwords
- Chat messages
- File names
- Server names
- News post titles
- Error messages

## C String (cstring)

**[UNCERTAIN]** KDX may use null-terminated C strings in some contexts.

### Structure

```
[char data[N]][null terminator]
```

- **N bytes**: Character data
- **1 byte**: Null terminator (0x00)
- Length determined by null terminator position

### Encoding

Variable length, terminated by `\0`

### When Used

**[TBD]** from binary analysis - may not be used in protocol at all

## Timestamps

**[TBD]** from KDXServer.lexe binary analysis

Typical formats might be:
- Unix timestamp (uint32): Seconds since 1970-01-01 00:00:00 UTC
- Custom format specific to KDX

## File Info Structure

**[TBD]** from binary analysis

Likely contains:
- File name (pstring)
- File size (uint32 or uint64)
- File type/creator (Mac-specific, 4 bytes each)
- Creation date (timestamp)
- Modification date (timestamp)
- Flags (uint16 or uint32)

## User Info Structure

**[TBD]** from binary analysis

Likely contains:
- User ID (uint16)
- Username (pstring)
- Icon ID (uint16)
- Status flags (uint16)
- Color (uint16 or RGB)

## Coordinate/Position

**[TBD]** from binary analysis

For GUI elements (if protocol includes positioning):
- X coordinate (int16 or uint16)
- Y coordinate (int16 or uint16)

## Color

**[TBD]** from binary analysis

Possible formats:
- RGB565 (uint16): 5 bits red, 6 bits green, 5 bits blue
- RGB888 (3x uint8): 8 bits per channel
- Indexed color (uint8): Palette index

## Flags and Bitfields

**[TBD]** from binary analysis

Common flag fields:
- User privileges (uint32): Bitfield of permissions
- File attributes (uint16): Read-only, hidden, etc.
- Server options (uint32): Feature flags

## Data Type Summary

| Type          | Size      | Encoding      | Notes                           |
|---------------|-----------|---------------|---------------------------------|
| uint8         | 1         | Binary        | Unsigned byte                   |
| uint16        | 2         | Big-endian    | Network byte order              |
| uint32        | 4         | Big-endian    | Network byte order              |
| int16         | 2         | Big-endian    | Signed, network byte order      |
| int32         | 4         | Big-endian    | Signed, network byte order      |
| pstring       | 1+N       | Length-prefix | **Primary string type**         |
| cstring       | N+1       | Null-term     | [UNCERTAIN] Rarely used?        |
| timestamp     | 4         | Big-endian    | [TBD] Unix or Mac epoch         |
| file_info     | Variable  | Structured    | [TBD] from analysis             |
| user_info     | Variable  | Structured    | [TBD] from analysis             |

## Serialization Guidelines

### Writing Data

1. Convert integers to network byte order (big-endian)
2. Write Pascal strings as length + data
3. Pack structures without padding (use `#pragma pack` or equivalent)
4. Write in order specified by protocol

### Reading Data

1. Read fixed-size fields first
2. Convert integers from network to host byte order
3. Read Pascal string length, then data
4. Validate lengths before reading variable data
5. Check buffer boundaries to prevent overruns

## Validation

### String Validation

```c
bool pstring_is_valid(const hx_pstring_t* pstr) {
    // Length must be reasonable
    if (pstr->length > 255) return false;
    
    // Optional: Check for valid characters
    for (int i = 0; i < pstr->length; i++) {
        if (pstr->data[i] < 0x20 && pstr->data[i] != '\t' && pstr->data[i] != '\n') {
            return false;  // Invalid control character
        }
    }
    
    return true;
}
```

### Integer Range Validation

```c
bool validate_uint16_range(uint16_t value, uint16_t min, uint16_t max) {
    return (value >= min && value <= max);
}
```

## Notes

- **Pascal strings are the primary string type** in KDX
- Always check buffer bounds when deserializing
- Validate lengths before allocating memory
- Be aware of endianness when working with multi-byte integers
- Update this document as more data structures are discovered

---

**Status**: Preliminary - Pascal strings confirmed, other types TBD  
**Last Updated**: 2026-05-29
