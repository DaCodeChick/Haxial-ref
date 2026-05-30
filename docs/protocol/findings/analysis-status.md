# KDX Binary Analysis Status

## Overview

This document tracks the progress of reverse engineering each KDX binary using Ghidra.

## Analysis Progress

### KDX Server (Mac)

**Binary**: `kdx_server_mac` (exact filename TBD)  
**Platform**: macOS (PowerPC or Intel)  
**Status**: Not Started  
**Progress**: 0%

**Completed Tasks**:
- [ ] Binary imported into Ghidra
- [ ] Initial auto-analysis complete
- [ ] Network functions identified
- [ ] Packet header format documented
- [ ] Pascal strings identified
- [ ] DAT_ globals renamed
- [ ] Transaction handlers mapped
- [ ] Authentication flow documented

**Findings Document**: [kdx-server-mac.md](kdx-server-mac.md)

---

### KDX Client (Mac)

**Binary**: `kdx_client_mac` (exact filename TBD)  
**Platform**: macOS (PowerPC or Intel)  
**Status**: Not Started  
**Progress**: 0%

**Completed Tasks**:
- [ ] Binary imported into Ghidra
- [ ] Initial auto-analysis complete
- [ ] Network functions identified
- [ ] Packet construction code found
- [ ] Pascal strings identified
- [ ] DAT_ globals renamed
- [ ] Transaction handling documented
- [ ] UI integration analyzed

**Findings Document**: [kdx-client-mac.md](kdx-client-mac.md)

---

### KDX Client (Windows)

**Binary**: `kdx_client_win.exe` (exact filename TBD)  
**Platform**: Windows (x86 or x64)  
**Status**: Not Started  
**Progress**: 0%

**Completed Tasks**:
- [ ] Binary imported into Ghidra
- [ ] Initial auto-analysis complete
- [ ] Network functions identified (Winsock)
- [ ] Cross-referenced with Mac client
- [ ] Platform differences documented
- [ ] Pascal strings identified
- [ ] DAT_ globals renamed
- [ ] Windows-specific code isolated

**Findings Document**: [kdx-client-win.md](kdx-client-win.md)

---

### KDX Tracker (Platform TBD)

**Binary**: `kdx_tracker` (exact filename TBD)  
**Platform**: TBD  
**Status**: Not Available  
**Progress**: 0%

**Completed Tasks**:
- [ ] Binary located
- [ ] Binary imported into Ghidra
- [ ] Initial auto-analysis complete
- [ ] Tracker protocol identified
- [ ] Server registration documented
- [ ] Query handling analyzed

**Findings Document**: [kdx-tracker.md](kdx-tracker.md)

---

## Overall Progress Summary

| Component          | Analysis | Network | Packets | Strings | Globals | Transactions | Total |
|--------------------|----------|---------|---------|---------|---------|--------------|-------|
| Server (Mac)       | 0%       | 0%      | 0%      | 0%      | 0%      | 0%           | 0%    |
| Client (Mac)       | 0%       | 0%      | 0%      | 0%      | 0%      | 0%           | 0%    |
| Client (Windows)   | 0%       | 0%      | 0%      | 0%      | 0%      | 0%           | 0%    |
| Tracker            | N/A      | N/A     | N/A     | N/A     | N/A     | N/A          | N/A   |

## Protocol Documentation Status

| Document            | Status       | Completeness |
|---------------------|--------------|--------------|
| packets.md          | Preliminary  | 10%          |
| data-types.md       | Preliminary  | 20%          |
| transactions.md     | Not Started  | 0%           |
| connection.md       | Not Started  | 0%           |
| constants.md        | Not Started  | 0%           |

## Next Steps

1. **Locate KDX binaries** for all three platforms
2. **Import into Ghidra** and run auto-analysis
3. **Start with Server (Mac)** as primary analysis target
4. **Identify network functions** (socket operations)
5. **Document packet header** format from send/receive code
6. **Find Pascal strings** in data sections
7. **Rename DAT_ globals** with descriptive names
8. **Map transaction handlers** and document opcodes

## Analysis Priorities

### High Priority
1. Packet header format
2. Authentication flow
3. Core transaction types (login, chat, file list)
4. Pascal string locations

### Medium Priority
1. File transfer protocol
2. News system
3. User management
4. Tracker protocol

### Low Priority
1. GUI-related code
2. Platform-specific utilities
3. Error handling details
4. Debug/logging code

## Timeline

**Target Milestones**:
- Import and initial analysis: TBD
- Basic packet format documented: TBD
- Core transactions documented: TBD
- Full protocol specification: TBD

## Notes

- Use AGENTS.md guidelines for consistent analysis approach
- Document findings incrementally - don't wait for complete analysis
- Mark uncertain findings with `[UNCERTAIN]` tag
- Cross-reference between Mac and Windows clients for validation
- Update this status document regularly

---

**Last Updated**: 2026-05-29  
**Next Review**: TBD
