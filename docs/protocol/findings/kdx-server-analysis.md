# KDXServer.lexe Analysis Notes

## Binary Information
- **File**: KDXServer.lexe  
- **Format**: ELF 32-bit LSB executable, Intel 80386
- **Language**: x86:LE:32:default
- **Compiler**: gcc
- **Image Base**: 0x08048000

## Entry Point Analysis

### entry (0x08049c64)
- Calls `__libc_start_main` with main function at 0x08091a5c

### main (0x08091a5c) [RENAMED]
```c
void main(uint param_1, undefined4 *param_2)
{
  server_init(param_1, param_2);
  FUN_08053a40();  // Main server loop
  if (DAT_080db6e4 != NULL) {
    (*DAT_080db6e4)();
  }
  _exit(0);
}
```

### server_init (0x08092198) [RENAMED]
- Handles command-line arguments:
  - `--nohup`: Sets up SIGHUP handler
  - `--bkgnd`: Forks into background
- Sets up signal handlers for SIGTERM(15), SIGPIPE(17), SIGCHLD(21), SIGCONT(22)
- Initializes timestamps using `gettimeofday()`

## Network Functions

### Socket Creation (0x08097380)
```c
void create_socket(int param_1, uint param_2, uint param_3, int param_4)
{
  // Creates TCP socket (type=1, proto=6) or UDP socket (type=2, proto=0x11)
  int socket_fd = socket(2, type, protocol);
  // Stores socket in data structure at offset 0x10
}
```

### Bind and Listen (0x08097d34)
```c
void bind_and_listen(int *socket_struct, uint16_t port)
{
  // Sets SO_REUSEADDR
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &one, 4);
  
  // Binds to port
  sockaddr_in addr;
  addr.sin_family = AF_INET (2);
  addr.sin_port = htons(port);
  bind(socket_fd, &addr, 16);
  
  // Listen with backlog of 1024
  listen(socket_fd, 0x400);
}
```

**Important**: Listen backlog is 1024 connections (0x400)

## Data Structures

### Socket Structure
Based on FUN_08097380:
- Offset 0x00: Magic value 0x42781ea5
- Offset 0x04: Some counter/ID
- Offset 0x10: Socket FD
- Offset 0x1c: Some parameter
- Offset 0x59: UDP flag (1 = UDP, 0 = TCP)

## Next Steps
- [ ] Find Pascal strings in data sections
- [ ] Identify packet send/receive functions
- [ ] Document packet format
- [ ] Find transaction handler dispatch
- [ ] Locate default port numbers

## Findings Date
2026-05-30

## Pascal Strings Discovery

### Location: .rodata section around 0x080adb00

**CONFIRMED**: KDX uses Pascal strings extensively as documented!

Format: `[uint8_t length][char data[length]]` - NO null terminator

### Examples Found:
- 0x080adb17: `05 45 72 72 6f 72` = length 5, "Error"
- 0x080adb1e: `16 4b 44 58 20 53 65 72 76 65 72...` = length 22, "KDX Server started on "
- 0x080adb72: `05 42 61 73 65 73` = length 5, "Bases"
- 0x080adb79: `07 44 65 66 61 75 6c 74` = length 7, "Default"
- 0x080adb82: `0c 47 65 6e 65 72 61 6c 20 4e 65 77 73` = length 12, "General News"
- 0x080adb90: `07 57 65 6c 63 6f 6d 65` = length 7, "Welcome"
- 0x080adb99: `03 4b 44 58` = length 3, "KDX"
- 0x080adba0: `0c 55 70 6c 6f 61 64 73 20 5b 55 4c 5d` = length 12, "Uploads [UL]"
- 0x080adbae: `12 41 64 6d 69 6e 20 44 72 6f 70 62 6f 78 20 5b 44 42 5d` = length 18, "Admin Dropbox [DB]"

### Context:
These appear to be default folder/newsgroup names for server initialization.

### Action Items:
- [ ] Apply Pascal string data type to all identified instances
- [ ] Rename globals that reference these strings
- [ ] Search entire .rodata section for more Pascal strings
- [ ] Look for Pascal strings in network packet handling code

