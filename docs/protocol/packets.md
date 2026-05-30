# KDX Packet Format

## Overview

KDX uses a binary packet format for all client-server communication. Each packet consists of a fixed-size header followed by variable-length payload containing parameter blocks and optional data.

## Packet Structure

```
+------------------+
| Packet Header    |  Fixed size
+------------------+
| Parameter Blocks |  Variable size (TLV format)
+------------------+
| Data Payload     |  Variable size (optional)
+------------------+
```

## Packet Header Format

**[TBD FROM BINARY ANALYSIS]** The following header format is a working hypothesis based on typical binary protocol patterns. Actual format must be confirmed through Ghidra analysis of KDXServer.lexe.

```
Offset | Size | Type       | Field          | Description
-------|------|------------|----------------|----------------------------------
0x00   | 4    | uint32_be  | magic          | Protocol magic/signature
0x04   | 2    | uint16_be  | type           | Packet type/transaction opcode
0x06   | 2    | uint16_be  | transaction_id | Transaction identifier
0x08   | 4    | uint32_be  | total_size     | Total packet size (including header)
0x0C   | 4    | uint32_be  | data_size      | Size of data payload section
0x10   | 4    | uint32_be  | param_count    | Number of parameter blocks
```

**Total Header Size**: 20 bytes (0x14)

### Field Descriptions

#### magic (0x00, 4 bytes)
- Protocol identification signature
- **[TBD]** Actual value from KDXServer.lexe binary analysis
- Used to validate packet format

#### type (0x04, 2 bytes)
- Transaction type / operation code
- Identifies the purpose of the packet
- See [transactions.md](transactions.md) for complete list
- Values range from 0x0000 to 0xFFFF

#### transaction_id (0x06, 2 bytes)
- Unique identifier for request/response matching
- Client assigns ID when sending request
- Server echoes same ID in response
- Allows asynchronous transaction handling

#### total_size (0x08, 4 bytes)
- Total packet size in bytes (header + parameters + data)
- Used for buffer allocation and validation
- Includes the 20-byte header itself

#### data_size (0x0C, 4 bytes)
- Size of the data payload section (after parameter blocks)
- May be 0 if packet contains only parameters
- Used for file transfers and large payloads

#### param_count (0x10, 4 bytes)
- Number of parameter blocks following the header
- May be 0 for simple packets
- Each parameter has its own TLV structure

## Parameter Block Format

Parameters use Type-Length-Value (TLV) encoding:

```
Offset | Size | Type      | Field       | Description
-------|------|-----------|-------------|---------------------------
0x00   | 2    | uint16_be | param_type  | Parameter type identifier
0x02   | 2    | uint16_be | param_size  | Size of parameter data
0x04   | N    | bytes     | param_data  | Parameter value (N bytes)
```

### Parameter Block Layout

```
[Type:2][Size:2][Data:N] [Type:2][Size:2][Data:N] ...
```

Multiple parameter blocks are concatenated directly after the header.

### Parameter Types

**[TBD]** Parameter type constants to be documented from KDXServer.lexe binary analysis.

Typical parameter types might include:

| Type  | Name          | Description                |
|-------|---------------|----------------------------|
| 0x0065| USERNAME      | User login name (pstring)  |
| 0x0066| PASSWORD      | User password (pstring)    |
| 0x0067| USER_ID       | User identifier (uint16)   |
| 0x0068| CHAT_MSG      | Chat message text (pstring)|
| 0x0069| FILE_NAME     | File name (pstring)        |
| ...   | ...           | [More TBD]                 |

See [transactions.md](transactions.md) for parameter types used by each transaction.

## Data Payload

Optional section following parameter blocks:
- Used for large data transfers (file content, etc.)
- Size specified in header's `data_size` field
- Format depends on transaction type
- May be binary data, not necessarily parameters

## Byte Order

**All multi-byte integers use network byte order (big-endian)**

Example conversion:
```c
uint32_t value = 0x12345678;
// Network byte order (big-endian): 12 34 56 78
// Host byte order may differ (little-endian on x86: 78 56 34 12)

// Use htonl() / ntohl() for conversion
uint32_t network = htonl(value);
```

## Example Packet

Hypothetical login request packet (format TBD from analysis):

```
Offset   | Hex                              | Description
---------|----------------------------------|---------------------------
0x00-0x03| 48 54 4C 50                      | Magic: "HTLP"
0x04-0x05| 00 6B                            | Type: Login (0x006B)
0x06-0x07| 00 01                            | Transaction ID: 1
0x08-0x0B| 00 00 00 2E                      | Total size: 46 bytes
0x0C-0x0F| 00 00 00 00                      | Data size: 0 (no payload)
0x10-0x13| 00 00 00 02                      | Param count: 2
0x14-0x15| 00 65                            | Param type: USERNAME
0x16-0x17| 00 05                            | Param size: 5 bytes
0x18-0x1C| 05 61 64 6D 69 6E                | Param data: pstring "admin"
0x1D-0x1E| 00 66                            | Param type: PASSWORD
0x1F-0x20| 00 09                            | Param size: 9 bytes
0x21-0x29| 08 70 61 73 73 77 6F 72 64       | Param data: pstring "password"
```

## Packet Construction Guidelines

1. **Create header** with appropriate fields
2. **Add parameter blocks** in TLV format
3. **Append data payload** if needed
4. **Calculate total_size** (header + params + data)
5. **Convert to network byte order** (big-endian)
6. **Send via TCP socket**

## Packet Parsing Guidelines

1. **Read header** (20 bytes)
2. **Validate magic** number
3. **Allocate buffer** based on total_size
4. **Read remaining data** (total_size - 20 bytes)
5. **Parse parameter blocks** based on param_count
6. **Extract data payload** if data_size > 0
7. **Convert from network byte order** as needed

## Error Handling

- **Invalid magic**: Reject packet, possibly close connection
- **Size mismatch**: Buffer overflow prevention
- **Unknown param types**: Skip or ignore based on implementation
- **Truncated packets**: Connection error, reconnect

## Notes

- This specification is preliminary and requires confirmation from binary analysis
- All structures and values are hypothetical until verified from KDXServer.lexe
- Mark confirmed values with Ghidra analysis references
- Update this document as analysis progresses

---

**Status**: Preliminary - Awaiting binary analysis of KDXServer.lexe  
**Last Updated**: 2026-05-29
