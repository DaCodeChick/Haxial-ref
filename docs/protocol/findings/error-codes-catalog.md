# Error Codes Catalog

## Error Codes Found in Binary

From log_error calls and socket_shutdown:

### Parameter/Validation Errors (0x1xxxx)
- **0x10002** - Invalid parameter (NULL pointer, zero size, etc.)
  - Used in: allocate_memory, socket_send_or_queue
- **0x10019** - Invalid magic number
  - Used in: socket_send_or_queue (checks socket magic)

### Memory Errors (0x2xxxx)
- **0x20065** - Memory allocation failure
  - Used in: allocate_memory (malloc failed)
  - Used in: socket_send_or_queue (queue entry alloc failed)

### Socket Errors (0x4xxxx)
- **0x40066** - Socket already closed
  - Used in: socket_send_or_queue (socket state is 0x1c)
- **0x4006e** - Socket closed error
  - Used in: socket_shutdown (default error when closing)

### Line Numbers Seen
These appear as second parameter to log_error (likely line numbers):
- 0x4e1, 0x4e6, 0x4eb, 0x507 (socket_send_or_queue)
- 0x220, 0x221, 0x225, 0x226, 0x22a, 0x22b, 0x235, 0x23a, 0x23b (socket_set_options)

### Source File IDs
Third parameter to log_error (likely source file identifiers):
- **0x4e** - Most common, appears in many functions
- **0x4d5343** - Seen in allocate_memory
- **0x20065** - Also used as source ID

## Socket States Found

### Confirmed States
- **0x1c** - Closed/shutdown state
  - Set in: socket_shutdown
  - Checked in: socket_send_or_queue

### States to Find
- Initial/uninitialized state
- Connected/open state  
- Connecting state
- Error state
- Others in state machine

## Next Steps

1. Search for all state assignments to socket+0x58
2. Extract all error codes from log_error calls
3. Look for error code definitions in .rodata or as immediates
4. Create comprehensive enums

