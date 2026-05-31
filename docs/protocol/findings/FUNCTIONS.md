# KDX Key Functions

## Network I/O

### socket_shutdown
**Address**: 0x08097c90  
**Purpose**: Gracefully shutdown a socket

```c
void socket_shutdown(SocketInfo *socket)
```

- Validates magic (0x42781ea5)
- Calls system `shutdown(socket_fd, SHUT_RDWR)`
- Sets socket_state to SOCKET_STATE_CLOSED (0x1c)
- Sets error_code to ERROR_SOCKET_CLOSED (0x4006e) if not already set

---

### socket_send_or_queue
**Address**: 0x08098104  
**Purpose**: Send data or queue if socket busy

```c
int socket_send_or_queue(SocketInfo *socket, void *data, size_t length)
```

- Validates magic
- Attempts `send()` system call
- If partial send: queues remaining data
- If EAGAIN/EWOULDBLOCK: queues all data
- Returns 0 on success, KDX error code on failure
- Calls `handle_send_error` on failure

---

### socket_flush_send_queue
**Address**: 0x08098d3c  
**Purpose**: Flush queued send data

```c
int socket_flush_send_queue(SocketInfo *socket)
```

- Walks send_queue_head linked list
- Attempts to send queued buffers
- Removes successfully sent entries
- Returns 0 when queue empty, error code otherwise

---

### handle_send_error
**Address**: 0x08098400  
**Purpose**: Convert errno to KDX error, update socket state

```c
void handle_send_error(SocketInfo *socket, int errno_value)
```

- Calls `errno_to_kdx_error(errno_value)`
- Sets socket->error_code
- Sets socket->last_errno
- May trigger socket shutdown

---

## Packet Handling

### validate_packet_header
**Address**: 0x0809a124  
**Purpose**: Validate incoming packet header

```c
int validate_packet_header(ConnectionState *connection, void *packet)
```

- Checks packet magic ("PXTP" = 0x50585450)
- Validates sequence numbers
- Checks handshake_state
- Returns error codes:
  - 0x40071: ERROR_INVALID_PACKET_MAGIC
  - 0x40072: ERROR_SEQUENCE_ERROR
  - 0x40073: ERROR_INVALID_HANDSHAKE
  - 0x40074: ERROR_PACKET_TOO_LARGE

---

## Connection Management

### handle_connection_handshake
**Address**: Unknown  
**Purpose**: Connection handshake state machine

- Manages handshake_state transitions
- Calls authentication callback
- Sends handshake packets

---

### connection_send_handshake_packet
**Address**: Unknown  
**Purpose**: Send handshake packet to client

- Uses cipher constant 0x4878
- Constructs handshake packet (format unknown)

---

### connection_send_handshake_response
**Address**: Unknown  
**Purpose**: Send handshake response

---

## Error Handling

### errno_to_kdx_error
**Address**: 0x08098570  
**Purpose**: Convert errno to KDXErrorCode

```c
KDXErrorCode errno_to_kdx_error(int errno_value)
```

- Maps system errno values to KDX error codes
- Many-to-one mappings (error coalescing)
- Returns ERROR_UNKNOWN (0xfffff) for unmapped values
- Implementation: `hx_errno_to_error()` in libhaxial

---

### log_error_with_details
**Address**: 0x080913c0  
**Purpose**: Log detailed error information

---

## Synchronization

### lock_release
**Address**: 0x08098c10  
**Purpose**: Release a lock

- Validates magic
- Releases pthread mutex or similar

---

## Event System

### event_set_fd_bit
**Address**: 0x08098d00  
**Purpose**: Set file descriptor bit in event mask

- Updates fd_set for select()
- Used in event loop

---

## Memory Management

### initialize_send_queue_pool
**Address**: 0x08098ea0  
**Purpose**: Initialize pool of send queue entries

- Pre-allocates queue entry structures
- Reduces malloc overhead during send

---

## Server Lifecycle

### server_initialize
**Address**: Unknown  
**Purpose**: Initialize server state

- Allocates main_socket and tracker_socket
- Loads databases (accounts_db, users_db, files_db)
- Loads news_list
- Initializes data_buffer (200KB)
- Sets up random IDs
- Calls helper functions:
  - signal_callback_handler
  - server_state_callback
  - load_resource_data
  - data_structure_insert
  - set_exit_flag
  - trigger_shutdown
  - log_error_with_details

---

### server_event_loop
**Address**: Unknown  
**Purpose**: Main server event loop

- Uses `select()` on socket fd_sets
- Dispatches I/O events
- Handles stdin (if not --nohup)

---

### handle_stdin_input
**Address**: 0x08056b24  
**Purpose**: Handle stdin commands (interactive mode)

---

### set_exit_flag
**Address**: 0x0807a000  
**Purpose**: Set flag to trigger shutdown

---

### trigger_shutdown
**Address**: 0x0807a018  
**Purpose**: Initiate graceful server shutdown

---

## Utility Functions

### binary_search
**Address**: 0x08052c48  
**Purpose**: Binary search in sorted array

```c
void* binary_search(void *key, void *base, size_t nmemb, size_t size,
                    int (*compar)(const void*, const void*))
```

- Standard bsearch() implementation
- Used for database lookups

---

### signal_callback_handler
**Address**: 0x08052f80  
**Purpose**: Handle POSIX signals (SIGTERM, SIGINT, etc.)

---

### server_state_callback
**Address**: 0x08053118  
**Purpose**: Callback for server state changes

---

### load_resource_data
**Address**: 0x0807baac  
**Purpose**: Load resource data from disk

---

### data_structure_insert
**Address**: 0x0808e010  
**Purpose**: Insert into data structure (tree? hash?)

---

## Notes

- Many functions not yet analyzed
- Packet construction/parsing functions need analysis
- Message dispatch mechanism unknown
- Database format unknown
