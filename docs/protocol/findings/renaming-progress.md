# Renaming Progress

## Critical Rule
**RENAME IMMEDIATELY** - As soon as you identify what something is, rename it RIGHT THEN.
- Functions
- Parameters  
- Local variables
- Globals
- Everything!

## Renamed So Far

### Functions (13)
- main (0x08091a5c)
- server_init (0x08092198)  
- server_main_loop (0x08053a40)
- server_initialize (0x080535b4)
- server_event_loop (0x0808fe38)
- create_socket (0x08097380)
- bind_and_listen (0x08097d34)
- socket_recv_packet (0x08098368)
- socket_send_packet (0x08098270)
- connection_receive_packets (0x08087024)
- connection_state_machine (0x08085a04)

### Globals (1)
- g_socket_list_or_context (0x080ddb30)

### Variables Renamed

**bind_and_listen**:
- param_1 → socket_struct
- param_2 → port
- local_20 → enable_reuse
- local_1c → addr

**socket_recv_packet**:
- param_1 → socket_struct
- param_2 → out_bytes_read
- param_3 → out_more_data

**socket_send_packet**:
- param_1 → socket_struct
- param_2 → data
- param_3 → length

**connection_receive_packets**:
- param_1 → connection

**connection_state_machine**:
- param_1 → connection

## Still Need to Rename

### Constants/Magic Numbers
- 0x42781ea5 - socket structure magic
- 0x2aa24aec - connection structure magic
- 0x4878 - encryption/obfuscation constant
- 0x29cc (10700) - server port
- 0x2a30 (10800) - tracker port

### Globals Referenced
- DAT_080dde20 - server context structure (passed to server_initialize)
- DAT_080db6e4 - cleanup function pointer (in main)
- DAT_080db718 - used in server_event_loop
- DAT_080de1c8 - loop exit flag
- DAT_080ddad0 - event loop running flag
- Many more in server_event_loop fd_set globals

### Next Functions to Analyze & Rename
- FUN_08066fd8 - error handler
- FUN_0809198c - error handler variant
- FUN_08071054 - memset/zero function
- FUN_0808fcbc - referenced multiple times
- FUN_08071bec - returns socket list/context
- FUN_080878c8 - packet reassembly (not renamed yet)
- FUN_08087104 - packet parsing helper
- FUN_0808723c - packet parsing helper
- FUN_080872f4 - connection handler
- FUN_0808751c - connection handler

## Strategy Going Forward
1. For each function analyzed, rename ALL parameters and locals IMMEDIATELY
2. Rename globals as soon as they're referenced
3. Re-decompile after renaming to see improved output
4. Continue with next function

