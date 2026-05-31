# KDX Analysis - Unknowns and Open Questions

This document tracks what we DON'T know yet. Items are removed as they're resolved.

## Struct Gaps

### SocketInfo (256 bytes total)
**Known**: 10 fields at offsets +0x00, +0x04, +0x10, +0x14, +0x18, +0x24, +0x44, +0x58, +0x59, +0x5b

**Unknown gaps**:
- **+0x08 to +0x0f**: 8 bytes
- **+0x1c, +0x20**: Pointers or values?
- **+0x28 to +0x43**: 28 bytes
- **+0x48 to +0x57**: 16 bytes
- **+0x5a, +0x5c+**: Remaining bytes to 256

**Questions**:
- What fields are in these gaps?
- Are there buffers embedded in the struct?
- Crypto contexts?

---

### ConnectionState
**Known**: magic (0x2aa24aec), socket pointer, handshake_state, callbacks, sequence numbers

**Unknown**: Almost everything
- Total size unknown
- Buffer locations unknown
- Crypto context storage unknown
- Full state machine fields unknown

**Questions**:
- Where are read/write buffers?
- Where is user data stored (username, etc.)?
- How is authentication state tracked?
- Where are Blowfish/SHA-1 contexts?

---

### ServerState (746 bytes)
**Known**: 10 named fields (sockets, databases, folders, buffers)

**Unknown gaps**:
- **+0x10**: Timer/handler from create_timer_or_handler
- **+0x64**: Another timer/handler
- **+0x98**: Pascal string (server description?)
- **+0x264, +0x268, +0x26c**: Three random IDs (purpose?)
- Many other offsets used but not understood

**Questions**:
- What are the random IDs used for?
- What are the timer/handler objects?
- Complete database structure?
- News list format?

---

## Protocol Unknowns

### Packet Structure
**Known**: Magic "PXTP", validated by validate_packet_header

**Unknown**:
- Complete header format (only 4 bytes known)
- Packet payload structure
- How message types are identified (opcodes? names? other?)
- Parameter/data encoding
- Length field location/format
- Checksum or CRC?

---

### Handshake Protocol
**Known**: State machine (INITIAL, RETRY, AUTHENTICATED, FAILED), uses cipher 0x4878

**Unknown**:
- Complete packet format
- Cipher algorithm (what is 0x4878 used for?)
- Challenge/response mechanism
- Key exchange (if any)
- Authentication data format

---

### Crypto Usage
**Known**: Blowfish and SHA-1 implemented

**Unknown**:
- When is Blowfish used? (handshake? session? file transfer?)
- What is SHA-1 used for? (password hashing? packet integrity? HMAC?)
- Key derivation?
- IV generation?

---

### Message Processing
**Known**: Packets validated, state machine exists

**Unknown**:
- How are different message types identified?
- Message dispatch mechanism
- Command/message structure
- Response format
- Error handling in messages

---

## Function Unknowns

### Unanalyzed Helper Functions
Many functions called but not yet analyzed:
- `FUN_08098e44` - Called on send error
- `FUN_0806edd8` - Adds to send queue
- `FUN_0808fcfc` - Signals event system
- Many others

---

### Global Variables

#### Timestamp Quartet (need batch analysis)
- **DAT_080ddaa8**: tv_usec value
- **DAT_080ddaa0**: Another usec value
- **DAT_080ddaac**: tv_sec value
- **DAT_080ddaa4**: Another sec value

**Questions**:
- Why two of each (sec and usec)?
- Used for deltas? Random seed? Logging?
- How are they related?

---

#### Pointer Globals
- **DAT_080de1d4**: Unknown pointer
- **DAT_080de1ec**: Another unknown pointer
- **DAT_080de1f0**: Initialized to 0 (flag? counter?)

**Questions**:
- What do they point to?
- Are they related?
- What subsystems use them?

---

#### Callback Globals
- **DAT_080db6e8**: Points to FUN_08092988
- **DAT_080db6ec**: Points to FUN_08092990

**Questions**:
- What are the callback functions for?
- When/how are they invoked?
- Signal handlers? Event callbacks?

---

#### Socket Global
- **DAT_080ddb30**: Passed to socket registration function

**Questions**:
- Socket list head?
- Event context?
- Socket manager object?

---

## Algorithm/Implementation Questions

### Blowfish (from decompilation)
- What are temp1/temp2/temp3 in the cipher?
- Why the specific bit rotation operations?
- Why local byte temps for output?

### SHA-1 (from decompilation)
- What is the "padding" variable for?
- Need to analyze helper functions (FUN_0807bc9c, FUN_0807bd98)

### Random Number Generation
- Why 256-byte pool specifically?
- Why FNV-1a + LCG combination?
- What are random IDs used for in ServerState?

---

## Database Format

**Known**: accounts_db, users_db, files_db exist

**Unknown**:
- File format
- Record structure
- Indexing mechanism
- Query interface
- Storage format (binary? text? custom?)

---

## Application-Level Protocol

**Unknown** (everything):
- File transfer mechanism
- Chat message format
- User list format
- News item format
- File listing format
- Permissions model
- Icon transfer
- Banner transfer

---

## Action Items

### High Priority
1. Complete packet header format analysis
2. Map ConnectionState structure completely
3. Identify message type discrimination mechanism
4. Understand crypto usage (when/where)

### Medium Priority
1. Complete SocketInfo gap analysis
2. Analyze remaining ServerState fields
3. Map database file format
4. Analyze unidentified helper functions

### Low Priority
1. Investigate timestamp global usage
2. Identify pointer global purposes
3. Understand callback registration
4. Document algorithm implementation details

---

## Notes

- This document is a working list - items are removed as they're resolved
- Don't invent answers - leave unknowns as unknowns
- Mark guesses clearly as [GUESS] in code/comments
- Update this as new unknowns are discovered
