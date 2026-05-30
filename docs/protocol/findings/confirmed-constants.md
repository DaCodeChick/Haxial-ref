# Confirmed Protocol Constants

## Network Ports

### Default Ports
- **Server Port**: 10700 (TCP)
- **Tracker Port**: 10800 (TCP)

Source: Confirmed from analysis

## Socket Magic Number
- **Socket Structure Magic**: `0x42781ea5`
  - Used for validation in socket wrapper functions
  - Found in: create_socket, bind_and_listen, socket_recv_packet, socket_send_packet

## Network Configuration
- **Listen Backlog**: 1024 connections (0x400)
  - Used in bind_and_listen at 0x08097d34

## Socket Options
- **SO_REUSEADDR**: Enabled on all server sockets
  - Set in bind_and_listen before binding

## Protocol Details (To Be Determined)
- [ ] Packet header structure
- [ ] Transaction identification method (IDs vs. names vs. other)
- [ ] Parameter encoding format
- [ ] Authentication mechanism
- [ ] Encryption (if any)

