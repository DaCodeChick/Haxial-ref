# KDX Server Analysis - Current State

## Quick Reference

**Binary**: KDXServer.lexe (Linux x86 32-bit)  
**Analysis Tool**: Ghidra + MCP  
**Policy**: See `AGENTS.md` for complete guidelines

## Naming Policy (TL;DR)

| Type | Unknown? | Action | Document? |
|------|----------|--------|-----------|
| **Parameters** | Yes | Leave as `param_1` | ✓ Required |
| **Globals** | Yes | Leave as `DAT_080xxxxx` | ✓ Required |
| **Locals** | Yes | Can use `temp`, `flags` | Optional |
| **Known** | No | Use specific name | N/A |

**Key Rule**: Only rename when you understand what something is.

## Current Progress

### Functions Analyzed: 29
All with proper names, struct types, and known parameters renamed.

### Structures Defined: 4
- **BlowfishContext** (4168 bytes) - Complete ✓
- **SHA1Context** (92 bytes) - Complete ✓
- **SocketInfo** (~90 bytes) - Partial, fields documented
- **ConnectionState** - Identified, needs mapping

### Globals Named: 4
- g_cleanup_callback ✓
- g_quiet_mode ✓
- g_sockets_initialized ✓
- g_malloc_count ✓

### Documented Unknowns: 50+
All in `docs/protocol/findings/unknowns-to-revisit.md`

## Key Files

### Analysis Guidelines
- **AGENTS.md** - Complete analysis methodology
- **docs/protocol/findings/unknowns-to-revisit.md** - All documented unknowns
- **docs/protocol/findings/final-policy-summary.md** - Complete naming policy

### Progress Documentation
- **docs/protocol/findings/struct-analysis-summary.md** - All structures
- **docs/protocol/findings/globals-correction-summary.md** - Globals handling
- **docs/protocol/findings/honest-analysis-achievement.md** - Overall progress

### Protocol Findings
- **docs/protocol/findings/confirmed-constants.md** - Known protocol values
- **docs/protocol/findings/helper-functions-renamed.md** - Helper functions

## Protocol Constants Discovered

- **Server port**: 10700
- **Tracker port**: 10800
- **Socket magic**: 0x42781ea5
- **Connection magic**: 0x2aa24aec
- **Listen backlog**: 1024
- **Buffer size**: 0x20000 (128KB)
- **Closed state**: 0x1c

## Cryptography Identified

- **Blowfish** - 16 rounds, handshake encryption
- **SHA-1** - Password hashing, authentication

## Next Steps

### High Priority
1. Analyze callback functions (DAT_080db6e8, DAT_080db6ec)
2. Complete SocketInfo mapping (resolve gaps)
3. Complete ConnectionState mapping (critical)
4. Analyze send queue management (4 helper functions)

### Process
1. Continue zero-tolerance renaming for knowns
2. Document unknowns immediately (params/globals)
3. Mark guesses explicitly with [GUESS]
4. Update unknowns doc as understanding improves

## Quick Start for New Analysis

1. **Read**: `AGENTS.md` for methodology
2. **Check**: `unknowns-to-revisit.md` for open questions
3. **Follow**: Three-tier naming (KNOWN/GUESS/UNKNOWN)
4. **Document**: All unknowns immediately

## Verification Checklist

Before committing new analysis:
- [ ] Known parameters renamed with specific names
- [ ] Unknown parameters left as param_1, documented
- [ ] Globals either properly named or left as DAT_*, documented
- [ ] Struct types applied to pointer parameters
- [ ] All unknowns have entries in unknowns-to-revisit.md
- [ ] Guesses marked with [GUESS]

## Contact / Questions

See `unknowns-to-revisit.md` for specific investigation questions.  
See `AGENTS.md` for analysis methodology questions.

