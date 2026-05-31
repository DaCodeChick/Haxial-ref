# ServerState Struct Proper Naming

**Date**: 2026-05-30  
**Issue**: ServerState had unhelpful placeholder names (g0x80, socket_2, etc.)  
**Solution**: Regenerated with all unnamed fields, then named ONLY understood fields

## Problem

The old ServerState struct had garbage placeholder names like:
- `g0x80` - meaningless hex offset name
- `socket_2` - generic numbered name
- Other unhelpful filler names

This violated AGENTS.md principle: **Only rename when you understand what something is**

## Solution

1. ✅ Regenerated ServerState with ALL unnamed fields (correct per AGENTS.md)
2. ✅ Analyzed `server_initialize` function to identify field usage
3. ✅ Named ONLY fields we actually understand from usage context

## Fields Named from server_initialize Analysis

| Offset | Type | Name | Evidence |
|--------|------|------|----------|
| +0x00 | SocketInfo* | main_socket | `create_socket(...,1)` result, used with `start_listening()` |
| +0x0c | SocketInfo* | tracker_socket | `create_socket(...,2)` result |
| +0x34 | LinkedListHead | news_list | Has `.head` member, used with `list_prepend()` |
| +0x48 | void* | default_folder | `create_folder(...,"Default",...)` result |
| +0x4c | void* | bases_folder | `create_base_folder(...,"Bases",...)` result |
| +0x58 | int* | accounts_db | `create_data_structure()`, used by `load_accounts()` |
| +0x5c | int* | users_db | `create_data_structure()` result |
| +0x60 | int* | files_db | `create_data_structure()` result |
| +0x2ac | void* | data_buffer | `allocate_memory(0x32000)` - 200KB buffer |
| +0x2e9 | byte | initial_setup_pending | Set to 1 when no accounts exist |

## Fields NOT Yet Named (Unknown)

Following AGENTS.md, these remain unnamed until we understand them:

| Offset | Type | Usage in server_initialize | Status |
|--------|------|---------------------------|---------|
| +0x10 | undefined8* | `create_timer_or_handler(...)` result | Timer/handler, purpose unclear |
| +0x64 | undefined8* | `create_timer_or_handler(...)` result | Another timer/handler |
| +0x98 | byte[?] | `pascal_string_copy(&param_1->field70_0x98,...)` | Pascal string, unknown purpose |
| +0x258 | uint | Set to `0x29cc` (10700 decimal) | Possibly port number |
| +0x264 | uint | `generate_random_id()` result | Random ID #1 |
| +0x268 | uint | `generate_random_id()` result | Random ID #2 |
| +0x26c | uint | `generate_random_id()` result | Random ID #3 |
| +0x278 | uint | Set to `0x2ba9` (11177 decimal) | Possibly tracker port |
| +0x2b0 | uint | Set to 0 | Buffer offset/size? |
| +0x2b4 | uint | Set to 0 | Buffer size/offset? |
| +0x2c8 | uint | Set to `0x100000` (1048576) | Max size/limit? |

## Decompilation Improvement

**Before** (with fake names):
```c
param_1->g0x80 = create_socket(...);
param_1->socket_2 = create_socket(...);
```

**After** (with proper names):
```c
param_1->main_socket = create_socket(...);
param_1->tracker_socket = create_socket(...);
```

Much more readable and honest about what we know vs. don't know.

## Next Steps

To name more fields:
1. Analyze other functions that use ServerState (server_event_loop, etc.)
2. Cross-reference field usage across multiple functions
3. Name fields only when confident about their purpose
4. Document unknowns in separate file for future investigation

## Status

- ✅ 10 fields properly named with evidence
- ✅ Remaining fields left unnamed (correct per AGENTS.md)
- ✅ Decompilation now shows clear, meaningful names
- ✅ Unknown fields properly documented for future analysis

This demonstrates the correct AGENTS.md workflow: regenerate to remove bad names, then name only what you understand.
