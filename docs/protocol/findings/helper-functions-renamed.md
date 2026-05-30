# Helper Functions Renamed

## Memory Management Functions ✓
1. **allocate_memory** (0x0808f6cc) - malloc wrapper with error checking
   - Increments g_malloc_count
   - Calls log_error on failure
   
2. **free_memory** (0x0808f7ec) - free wrapper
   - Decrements g_malloc_count
   - Nulls the pointer reference

3. **memcpy_wrapper** (0x080709c8) - memmove wrapper
   - Returns size copied

## Comparison Functions ✓
4. **memory_compare** (0x0806f158) - optimized memcmp
   - Aligns to 4-byte boundaries
   - Compares words when possible
   - Returns 1 if equal, 0 if not

## Cryptographic Functions ✓
5. **sha1_hash** (0x0807bbfc) - SHA-1 hash function
   - Standard SHA-1 initialization constants
   - Calls FUN_0807bc9c and FUN_0807bd98 (SHA-1 update/finalize)

6. **blowfish_encrypt** (0x0806565c) - Blowfish block cipher
   - 16-round Feistel network
   - Outputs 16 bytes (128 bits)

## Socket Functions ✓
7. **socket_shutdown** (0x08097c90) - graceful socket shutdown
   - Checks socket magic 0x42781ea5
   - Calls shutdown(fd, SHUT_RDWR)
   - Sets socket_state to 0x1c
   - Sets error code 0x4006e if not already set

8. **socket_send_or_queue** (0x08098104) - send with queue fallback
   - Tries send() first
   - If can't send all, queues remaining
   - Calls FUN_08098d3c (allocate send queue entry)
   - Calls FUN_0806edd8 (add to send queue)
   - Calls FUN_08098ea0 (flush send queue)
   - Calls FUN_0808fcfc (signal event system)

9. **socket_set_options** (0x08099354) - configure socket options
   - Sets O_NONBLOCK (fcntl F_SETFL)
   - Sets FD_CLOEXEC (fcntl F_SETFD)
   - Sets SO_SNDBUF/SO_RCVBUF to 0x20000 (128KB)
   - Sets SO_LINGER to 0 (immediate close)
   - For TCP: Sets TCP_NODELAY and SO_KEEPALIVE

## Still TODO (referenced but not yet analyzed)
- FUN_0807bc9c - SHA-1 update
- FUN_0807bd98 - SHA-1 finalize
- FUN_08098d3c - allocate send queue entry
- FUN_0806edd8 - add to send queue
- FUN_08098ea0 - flush send queue
- FUN_0808fcfc - signal/wake event system
- FUN_08098e44 - handle send error
- FUN_08091820 - convert errno to error code
- FUN_080918c4 - log error variant
- FUN_08066eec - error handler variant
- FUN_0808f788 - allocate socket structure
- FUN_0808fae8 - register socket in event system
- FUN_0806efa4 - add to global socket list
- FUN_0807aac0 - get random number
- FUN_08092998 - SIGHUP handler
- FUN_08092988 - callback 1
- FUN_08092990 - callback 2
- FUN_080927a0 - SIGTERM handler
- FUN_080927ac - SIGPIPE handler
- FUN_080927b8 - process program path
- FUN_080979e0 - check socket state
- FUN_080876e8 - get packet size
- FUN_08097cd8 - close socket variant
- FUN_0808f724 - duplicate/copy buffer

## Key Discoveries
- **Socket magic**: 0x42781ea5
- **Connection magic**: 0x2aa24aec (from earlier analysis)
- **Socket state 0x1c**: Closed/shutdown state
- **Error code 0x4006e**: Socket closed error
- **Buffer sizes**: 128KB for send/receive
- **Crypto**: SHA-1 + Blowfish used in handshake

