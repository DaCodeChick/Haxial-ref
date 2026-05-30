# Unnamed Functions Called by Named Functions

## From create_socket
- FUN_08066eec - error handler variant
- FUN_0808f788 - allocate memory (returns socket structure)
- FUN_08099354 - socket configuration
- FUN_0808fae8 - register socket in event system
- FUN_0806efa4 - add to global socket list (&DAT_080ddb20)

## From parse_args_and_setup_signals
- FUN_080927b8 - process program path
- FUN_0806f158 - string compare
- FUN_08092998 - SIGHUP handler
- FUN_08092988 - callback 1
- FUN_08092990 - callback 2
- FUN_080927a0 - SIGTERM handler
- FUN_080927ac - SIGPIPE handler

## From handle_connection_handshake
- FUN_080979e0 - check socket state?
- FUN_080876e8 - get packet size?
- FUN_0806f158 - string/data compare
- FUN_0807bbfc - hash function
- FUN_0806565c - crypto/hash operation
- FUN_08097c90 - close/cleanup socket
- FUN_08097cd8 - close socket variant

## From connection_send_handshake_response
- FUN_0808f6cc - allocate buffer
- FUN_0807aac0 - get random number
- FUN_080709c8 - memcpy wrapper
- FUN_08098104 - send on socket (another send variant?)

## From connection_send_handshake_packet
- FUN_0807bbfc - hash function (same as above)
- FUN_0806565c - crypto/hash (same as above)

## From socket_recv_packet
- FUN_0808f6cc - allocate buffer (same)
- FUN_0808f7ec - free buffer
- FUN_080918c4 - log error variant

## From socket_send_packet
- FUN_0808f724 - duplicate/copy buffer?
- FUN_08098d3c - allocate send queue entry?
- FUN_0806edd8 - add to send queue
- FUN_08098ea0 - flush send queue?
- FUN_0808fcfc - signal/wake event system?
- FUN_08098e44 - handle send error?

## From connection_consume_bytes
- FUN_0808f7ec - free buffer (same)
- FUN_080709c8 - memcpy (same)

## From log_socket_error
- FUN_08091820 - convert errno to error code

## Priority Order for Renaming
1. Memory functions: FUN_0808f6cc, FUN_0808f7ec, FUN_0808f724
2. String/memory: FUN_080709c8, FUN_0806f158
3. Socket I/O: FUN_08098104, FUN_08099354, FUN_08097c90
4. Crypto/hash: FUN_0807bbfc, FUN_0806565c
5. Event system: FUN_0808fae8, FUN_0806efa4, FUN_0808fcfc
6. Error handling: FUN_08066eec, FUN_08091820, FUN_080918c4

