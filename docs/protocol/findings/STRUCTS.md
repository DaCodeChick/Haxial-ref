# KDX Structs

## SocketInfo

**Size**: 256 bytes  
**Magic**: 0x42781ea5  
**Location**: Allocated dynamically, managed by server

### Known Fields

| Offset | Type | Name | Purpose |
|--------|------|------|---------|
| +0x00 | uint | magic | Magic number (0x42781ea5) for validation |
| +0x04 | uint | event_context | Event system handle |
| +0x10 | uint | socket_fd | File descriptor |
| +0x14 | KDXErrorCode | error_code | KDX error code |
| +0x18 | int | last_errno | Last errno from system calls |
| +0x24 | int | send_queue_head | Send queue linked list head |
| +0x44 | uint | bytes_sent | Partial send tracking |
| +0x58 | SocketState | socket_state | Socket state enum |
| +0x59 | byte | is_udp | UDP flag |
| +0x5b | byte | shutdown_pending | Graceful shutdown pending |

### Unknown Fields
- +0x08 to +0x0f: 8 bytes (gap)
- +0x1c, +0x20: Unknown pointers/values
- +0x28 to +0x43: 28 bytes (gap)
- +0x48 to +0x57: 16 bytes (gap)
- +0x5a, +0x5c+: Remaining fields

### Functions Using SocketInfo
- socket_shutdown
- socket_send_or_queue
- socket_flush_send_queue
- handle_send_error
- socket_set_options

---

## ConnectionState

**Size**: Unknown (at least several hundred bytes)  
**Magic**: 0x2aa24aec  

### Known Fields

| Offset | Type | Name | Purpose |
|--------|------|------|---------|
| +0x00 | uint | magic | Magic number (0x2aa24aec) |
| +0x04 | uint | (unnamed) | Connection/session ID |
| +0x0c | SocketInfo* | socket | Pointer to socket |
| +0x88 | function* | (unnamed) | Authentication callback |
| +0x8c | void* | (unnamed) | Auth callback user data |
| +0x94 | uint | (unnamed) | Flags (bit 2 = error flag) |
| +0x98 | byte | handshake_state | Handshake state machine |
| +0x99 | byte | (unnamed) | Expected sequence number |
| +0x9a | byte | (unnamed) | Received sequence number |

### Functions Using ConnectionState
- validate_packet_header
- connection_send_handshake_packet
- connection_send_handshake_response
- handle_connection_handshake

---

## ServerState

**Size**: 746 bytes  
**Location**: Global at 0x080dde20 (g_server_state)

### Known Fields

| Offset | Type | Name | Purpose |
|--------|------|------|---------|
| +0x00 | SocketInfo* | main_socket | Main listening socket |
| +0x0c | SocketInfo* | tracker_socket | Tracker socket |
| +0x34 | LinkedListHead | news_list | News items list |
| +0x48 | void* | default_folder | Default folder handle |
| +0x4c | void* | bases_folder | Bases folder handle |
| +0x58 | int* | accounts_db | Accounts database |
| +0x5c | int* | users_db | Users database |
| +0x60 | int* | files_db | Files database |
| +0x2ac | void* | data_buffer | 200KB data buffer (0x32000) |
| +0x2e9 | byte | initial_setup_pending | Initial setup flag |

### Unknown Fields (Need Analysis)

| Offset | Type | Usage | Notes |
|--------|------|-------|-------|
| +0x10 | undefined8* | Timer/handler | From create_timer_or_handler |
| +0x64 | undefined8* | Timer/handler | Another timer/handler |
| +0x98 | byte[?] | Pascal string | Server description? |
| +0x258 | uint | Set to 0x29cc | Port? (10700 decimal) |
| +0x264 | uint | random_id() | Random ID #1 |
| +0x268 | uint | random_id() | Random ID #2 |
| +0x26c | uint | random_id() | Random ID #3 |
| +0x278 | uint | Set to 0x2ba9 | Tracker port? (11177 decimal) |
| +0x2b0 | uint | Set to 0 | Buffer offset? |
| +0x2b4 | uint | Set to 0 | Buffer size? |
| +0x2c8 | uint | Set to 0x100000 | Max size? (1MB) |

### Functions Using ServerState
- server_initialize
- server_event_loop
- load_accounts
- load_file_catalog
- load_news_items

---

## LinkedListHead

**Size**: 16 bytes  
**Type**: Intrusive doubly-linked list

```c
struct LinkedListHead {
    void *head;  // +0x00
    void *tail;  // +0x04
    // 8 more bytes (count? padding?)
};
```

### Functions
- list_append
- list_prepend
- list_pop_head
- dlist_append

---

## Enums

### SocketState
- SOCKET_STATE_CLOSED = 0x1c
- SOCKET_STATE_CONNECTED = 0x01

### KDXErrorCode
See `PROTOCOL.md` for complete list (27 error codes)

---

## Notes

- **AGENTS.md Policy**: Fields remain unnamed until we understand their purpose
- Gaps in structs indicate unanalyzed regions
- Offset documentation preserved for future field identification
