# Packet Handling Flow

## Call Chain

1. **server_main (0x08091a5c)**
   - Calls server_init
   - Calls server_main_loop

2. **server_main_loop (0x08053a40)**
   - Calls FUN_08097318
   - Calls FUN_080967a0
   - Calls server_initialize with &DAT_080dde20
   - Calls server_event_loop

3. **server_initialize (0x080535b4)**
   - Initializes server state
   - Creates TCP socket: `create_socket(0x8052ad0, &DAT_080dde20, 0, 1)`
   - Creates UDP socket: `create_socket(0x8053a68, &DAT_080dde20, 0, 2)`

4. **server_event_loop (0x0808fe38)**
   - Main select() loop
   - Monitors fd_sets for read/write/exception
   - Dispatches to registered handlers via function pointers

5. **connection_receive_packets (0x08087024)**
   - Calls socket_recv_packet to get data
   - Accumulates packets in buffer
   - When limit reached (0x80 packets), calls FUN_080878c8

6. **FUN_080878c8 (packet reassembly)**
   - Combines fragmented packet buffers
   - Reassembles complete packets
   - [Need to trace where reassembled packets go]

## Socket Functions

- **create_socket (0x08097380)**: Creates TCP or UDP socket
- **bind_and_listen (0x08097d34)**: Binds socket and listens
- **socket_recv_packet (0x08098368)**: Receives packet from socket
- **socket_send_packet (0x08098270)**: Sends packet to socket

## Next Steps

- [ ] Find transaction dispatch table/switch
- [ ] Identify packet header parsing
- [ ] Document transaction opcodes
- [ ] Map transaction handlers

