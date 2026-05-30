# Unknowns to Revisit

This document tracks variables/parameters that have been given temporary generic names but need better names once we understand their purpose better.

## Format
```
Function: function_name (address)
Variable: current_name
Type: parameter/local
Temporary Name Given: generic_name
Notes: What we know so far, what to look for
Status: PENDING/IN_PROGRESS/RESOLVED
```

## List

(This will be populated as we encounter unknowns during renaming)


## Session 2 - Additional Unknowns from Struct Analysis

### blowfish_encrypt (0x0806565c)
- **left, right** - Blowfish L/R registers (standard algorithm)
- **temp1, temp2, temp3** - UNKNOWN: Intermediate values in Feistel network, purpose unclear
- **f_result, f_result2** - UNKNOWN: Results of F-function, exact role in rounds unclear
- **f_temp** - UNKNOWN: Temporary F-function value
- **byte_temp1, byte_temp2, byte_temp3** - UNKNOWN: Used for byte-by-byte output writing, why needed?

**Questions:**
- Why the bit rotation operations (>> 1, << 0x1f)?
- What are temp1/temp2/temp3 actually doing in the cipher?
- Why local byte temps instead of direct writes?

### sha1_hash (0x0807bbfc)
- **padding** - UNKNOWN: Purpose unclear, seems unused?
- **FUN_0807bc9c** - UNKNOWN: Called with ctx, data, length - SHA-1 update?
- **FUN_0807bd98** - UNKNOWN: Called with ctx, digest - SHA-1 finalize?

**Questions:**
- What is the padding variable for?
- Need to analyze the two helper functions

### socket_send_or_queue (0x08098104)
- **param_1[1]** - Named "event_context" but what is it exactly? Event system handle?
- **param_1[9]** - Named "send_queue_head" but is it a pointer to queue struct? What's the layout?
- **param_1[0x11]** - Named "bytes_sent" but is it sent so far, or offset, or what?
- **FUN_08098e44** - UNKNOWN: Called on send error, what does it do?
- **FUN_08098d3c** - UNKNOWN: Allocates queue entry, what's the struct?
- **FUN_0806edd8** - UNKNOWN: Adds to queue, how does queue work?
- **FUN_08098ea0** - UNKNOWN: Flush send queue, what's the logic?
- **FUN_0808fcfc** - UNKNOWN: Signal event system, how?

**Questions:**
- What is the send queue entry structure?
- How does the event system work?
- What happens in the error handler?

### socket_set_options (0x08099354)
- **flags, temp_flags, extra_flags, extra_flags2** - GENERIC NAMES: What are these flags actually for?
- **result, extra_result** - GENERIC NAMES: These are fcntl/setsockopt results, but using generic name
- **linger_struct, linger_time** - Actual meaning: SO_LINGER option structure
- **buffer_size** - Actual meaning: 0x20000 (128KB) buffer size

**Questions:**
- Why are there "extra" variables from Ghidra (extraout_EAX)?
- What do the fcntl flag operations actually do?
- Is 0x800 O_NONBLOCK on this system?

### SocketInfo Structure - Unknown Fields
- **+0x04 lock_or_refcount** - GUESS: Could be lock, could be refcount, which is it?
- **+0x08 event_context** - GUESS: Assumed event system context, but what type?
- **gap_0x0c[8]** - UNKNOWN: 8 bytes at +0x0c to +0x13, what's in here?
- **gap_0x18[20]** - UNKNOWN: 20 bytes at +0x18 to +0x23, what fields?
- **gap_0x28[28]** - UNKNOWN: 28 bytes at +0x28 to +0x43, what fields?
- **gap_0x48[16]** - UNKNOWN: 16 bytes at +0x48 to +0x57, what fields?
- **gap_0x5a[?]** - UNKNOWN: Everything after +0x5a, total size unknown

**Critical Questions:**
- What is the actual total size of SocketInfo?
- What fields are in the gaps?
- Is +0x04 a lock or refcount?
- What type is event_context exactly?

### ConnectionState Structure - Almost Completely Unknown
- **+0x00 magic** - Known: 0x2aa24aec
- **Everything else** - UNKNOWN: Need to map all fields

**Critical Questions:**
- What is the total size?
- Where are read/write buffers?
- Where is connection state machine state?
- Where are crypto contexts stored?

### Global Variables - Possibly Misnamed
- **g_data_ptr, g_data_ptr_2** - GENERIC NAMES: What are these actually?
- **g_zero_init** - GENERIC NAMES: What is this for?
- **g_callback_1, g_callback_2** - GENERIC NAMES: What do these callbacks do?
- **g_timestamp_usec_copy, g_timestamp_msec_copy** - Why copies? What's the purpose?

**Questions:**
- What do the data pointers point to?
- Why are there timestamp copies?
- What are the callbacks for?

## Action Items

1. **Analyze helper functions** to understand what temp variables actually represent
2. **Map SocketInfo completely** - find all fields in gaps
3. **Map ConnectionState completely** - critical for protocol understanding
4. **Revisit generic names** - replace with specific names once understood
5. **Document assumptions** - mark all guesses as [GUESS] in comments
6. **Cross-reference usage** - see how unknowns are used in other functions


## Reverted Globals - Need Investigation

### parse_args_and_setup_signals (0x08092198)

#### DAT_080ddaa8 - Previously named "g_timestamp_usec"
- **What we know**: Assigned `time_val.tv_usec` from gettimeofday
- **What we don't know**: Why stored globally? What uses it?
- **Questions**:
  - Is this for logging timestamps?
  - Is it used in random number generation?
  - Is it used for performance timing?
- **TODO**: Search for all XREFS to 0x080ddaa8

#### DAT_080ddaa0 - Previously named "g_timestamp_usec_copy"  
- **What we know**: Also gets a usec value
- **What we don't know**: Why two different usec globals? What's the relationship?
- **Questions**:
  - Is one a backup? Previous value?
  - Are they used for delta calculations?
  - Different purposes?
- **Related**: DAT_080ddaa8, DAT_080ddaac, DAT_080ddaa4
- **TODO**: Analyze all four timestamp-related globals together

#### DAT_080ddaac - Previously named "g_timestamp_msec"
- **What we know**: Assigned `time_val.tv_sec` from gettimeofday  
- **What we don't know**: Purpose of global storage
- **Related**: Part of timestamp quartet with DAT_080ddaa8, DAT_080ddaa0, DAT_080ddaa4

#### DAT_080ddaa4 - Previously named "g_timestamp_msec_copy"
- **What we know**: Also gets a sec value
- **What we don't know**: Why duplicate? Relationship to DAT_080ddaac?
- **Related**: Part of timestamp quartet

#### DAT_080de1d4 - Previously named "g_data_ptr"
- **What we know**: Some kind of pointer, set in parse_args_and_setup_signals
- **What we don't know**: What does it point to? Type? Purpose?
- **Questions**:
  - Data structure pointer?
  - Configuration?
  - Global state?
- **TODO**: Find all XREFS and see how it's used

#### DAT_080de1ec - Previously named "g_data_ptr_2"
- **What we know**: Another pointer
- **What we don't know**: What does it point to? Why two pointers?
- **Questions**:
  - Related to DAT_080de1d4?
  - Different subsystems?
  - Backup/alternative?
- **TODO**: Compare usage patterns with DAT_080de1d4

#### DAT_080de1f0 - Previously named "g_zero_init"
- **What we know**: Initialized to 0
- **What we don't know**: What is it? Flag? Counter? State?
- **Questions**:
  - Boolean flag?
  - Integer counter?
  - State machine value?
- **TODO**: Find where it's read/written after initialization

#### DAT_080db6e8 - Previously named "g_callback_1"
- **What we know**: Set to address of FUN_08092988
- **What we don't know**: What is FUN_08092988? Callback for what event/condition?
- **Questions**:
  - Signal handler?
  - Event callback?
  - Cleanup function?
- **TODO**: Analyze FUN_08092988 first
- **Related**: DAT_080db6ec (another callback)

#### DAT_080db6ec - Previously named "g_callback_2"  
- **What we know**: Set to address of FUN_08092990
- **What we don't know**: What is FUN_08092990? Callback for what?
- **Questions**:
  - Related to DAT_080db6e8?
  - Different event type?
  - Alternative callback?
- **TODO**: Analyze FUN_08092990 first
- **Related**: DAT_080db6e8

### create_socket (0x08099bd8)

#### DAT_080ddb30 - Previously named "g_socket_list_or_context"
- **What we know**: Passed to FUN_0806efa4 along with socket
- **What we don't know**: Is it a list head? Context struct? Manager object?
- **Questions**:
  - If list: Linked list of all sockets?
  - If context: What state does it hold?
  - If manager: What operations?
- **Observations**:
  - First parameter to FUN_0806efa4
  - Socket seems to be added to/registered with this
- **TODO**: Analyze FUN_0806efa4 to understand what it does with this parameter

## Globals That Are Actually Known (Kept Named)

### From main
- **g_cleanup_callback** (0x080db6e4) - ✓ Function pointer called at exit

### From parse_args_and_setup_signals  
- **g_quiet_mode** (0x080db6e2) - ✓ Boolean flag for --nohup

### From create_socket
- **g_sockets_initialized** (0x080ddafc) - ✓ Boolean initialization flag

### From allocate_memory
- **g_malloc_count** (0x080db6f8) - ✓ Counter for allocations

## Action Items

1. **Timestamp globals**: Analyze all four together (DAT_080ddaa8, DAT_080ddaa0, DAT_080ddaac, DAT_080ddaa4)
   - Find all XREFS
   - Look for usage patterns (delta calculations? random seed? logging?)
   - Determine why two of each (sec and usec)

2. **Pointer globals**: Investigate DAT_080de1d4 and DAT_080de1ec
   - What do they point to?
   - How are they used?
   - Are they related?

3. **Callback globals**: Analyze the callback functions first
   - FUN_08092988 (DAT_080db6e8)
   - FUN_08092990 (DAT_080db6ec)
   - Then understand when/why they're called

4. **Socket global**: Analyze FUN_0806efa4
   - Understand what DAT_080ddb30 represents
   - Is it a list? context? manager?

5. **Zero init global**: Find all reads/writes to DAT_080de1f0
   - Determine type (flag, counter, state)
   - Understand purpose

