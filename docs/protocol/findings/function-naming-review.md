# Function Naming Review

## Confusing Names

### server_init (0x08092198)
**Purpose**: Handles command-line arguments (--nohup, --bkgnd), sets up signal handlers, forks to background if requested
**Better Name**: `parse_args_and_setup_signals`

### server_initialize (0x080535b4)
**Purpose**: Called from server_main_loop, initializes server state, creates sockets, sets up network
**Better Name**: Keep as `server_initialize` (this is the actual initialization)

### server_main_loop (0x08053a40)
**Purpose**: Just calls a few setup functions then enters event loop
**Current**: Calls server_initialize then server_event_loop
**Better Name**: This IS the main loop orchestrator, name is fine

## Proposed Changes
- server_init → parse_args_and_setup_signals
- server_initialize → keep (or maybe init_server_context)
- server_main_loop → keep

