# Globals Correction Summary

## What Was Fixed

Reverted 10 improperly named globals back to DAT_* and documented the unknowns.

## Reverted Globals (10)

### Timestamp-Related (4)
1. **g_timestamp_usec** → DAT_080ddaa8
2. **g_timestamp_usec_copy** → DAT_080ddaa0  
3. **g_timestamp_msec** → DAT_080ddaac
4. **g_timestamp_msec_copy** → DAT_080ddaa4

**Why reverted**: 
- Don't know why timestamps are stored globally
- Don't know why there are "copies"
- Names like "copy" are assumptions, not facts

### Generic Pointers (3)
5. **g_data_ptr** → DAT_080de1d4
6. **g_data_ptr_2** → DAT_080de1ec
7. **g_zero_init** → DAT_080de1f0

**Why reverted**:
- "data_ptr" is meaningless - what data?
- "_2" implies relationship we haven't verified
- "zero_init" just describes initialization, not purpose

### Callbacks (2)
8. **g_callback_1** → DAT_080db6e8
9. **g_callback_2** → DAT_080db6ec

**Why reverted**:
- Haven't analyzed FUN_08092988 or FUN_08092990 yet
- Don't know what events they handle
- "_1" and "_2" are meaningless

### Socket-Related (1)
10. **g_socket_list_or_context** → DAT_080ddb30

**Why reverted**:
- "or" in the name = we don't actually know
- Could be list, context, manager, or something else
- Name encodes uncertainty instead of documenting it

## Properly Named Globals (4 - Kept)

These we actually understand:

1. **g_cleanup_callback** (0x080db6e4) ✓
   - Called at program exit
   - Clear purpose from usage

2. **g_quiet_mode** (0x080db6e2) ✓
   - Set by --nohup flag
   - Used to suppress output

3. **g_sockets_initialized** (0x080ddafc) ✓
   - Boolean flag
   - Prevents double initialization

4. **g_malloc_count** (0x080db6f8) ✓
   - Incremented on malloc
   - Decremented on free
   - Clear allocation counter

## The Pattern of Bad Names

### Anti-Pattern 1: Descriptive But Generic
- `g_data_ptr` - "data" is too vague
- `g_zero_init` - describes initialization, not purpose
- `g_callback_1` - "callback" without context

### Anti-Pattern 2: Numbered Variants
- `g_data_ptr_2` - implies relationship via numbering
- `g_callback_1`, `g_callback_2` - numbered but what for?
- `g_timestamp_usec_copy` - "copy" implies relationship

### Anti-Pattern 3: Uncertainty Encoding
- `g_socket_list_or_context` - "or" means we don't know
- Should be: DAT_* + documentation of possibilities

## Documentation Added

All 10 reverted globals now have entries in `unknowns-to-revisit.md` with:
- What we know (from code observation)
- What we don't know (gaps in understanding)
- Questions to investigate
- TODO items for next steps
- Cross-references to related unknowns

## Examples of Good Documentation

### DAT_080db6e8 (was "g_callback_1")
```markdown
- **What we know**: Set to address of FUN_08092988
- **What we don't know**: What is FUN_08092988? Callback for what event?
- **Questions**: Signal handler? Event callback? Cleanup function?
- **TODO**: Analyze FUN_08092988 first
- **Related**: DAT_080db6ec (another callback)
```

### DAT_080ddb30 (was "g_socket_list_or_context")
```markdown
- **What we know**: Passed to FUN_0806efa4 along with socket
- **What we don't know**: Is it a list head? Context struct? Manager object?
- **Questions**: 
  - If list: Linked list of all sockets?
  - If context: What state does it hold?
- **TODO**: Analyze FUN_0806efa4 to understand this parameter
```

## Impact on Code Readability

### Before (False Confidence)
```c
void parse_args_and_setup_signals(...) {
    gettimeofday(&time_val, &time_zone);
    g_timestamp_usec = time_val.tv_usec;
    g_timestamp_usec_copy = ...;  // Why "copy"? Don't really know
    
    DAT_080db6e8 = FUN_08092988;  // Now callback_1, but what callback?
}
```

### After (Honest Assessment)
```c
void parse_args_and_setup_signals(...) {
    gettimeofday(&time_val, &time_zone);
    DAT_080ddaa8 = time_val.tv_usec;  // TODO: Why stored globally?
    DAT_080ddaa0 = ...;                // TODO: Relationship to DAT_080ddaa8?
    
    DAT_080db6e8 = FUN_08092988;      // TODO: Analyze this function
}
```

The second version makes it obvious what needs investigation.

## Lessons Learned

1. **"copy" implies knowledge we don't have**
   - We see two variables storing similar data
   - We don't know if one is a "copy" of the other
   - Better: Note the pattern, investigate the relationship

2. **Numbered suffixes hide relationship**
   - `_1`, `_2` implies we understand the grouping
   - Often we just see multiple similar things
   - Better: Leave as DAT_*, note the pattern

3. **"or" names encode uncertainty**
   - `list_or_context` admits we don't know
   - Better: Leave as DAT_*, list possibilities in doc

4. **Generic + pointer = meaningless**
   - `data_ptr` - what data?
   - `ptr` - to what?
   - Better: Leave as DAT_*, investigate what it points to

## Verification

To verify globals are properly named:
- [ ] Name clearly indicates purpose without ambiguity
- [ ] Name doesn't use "or", "_1", "_2", "copy", etc.
- [ ] We've seen enough usage to understand it
- [ ] We haven't just observed initialization

If any of these fail → revert to DAT_*, document the unknown

## Statistics Update

### Before Correction:
- Globals renamed: 17
- Actually understood: 4
- Falsely confident: 10
- Uncertain ("or" names): 3

### After Correction:
- Properly named: 4 ✓
- Reverted to DAT_*: 10 ✓
- Documented unknowns: 10 ✓
- Honest assessment achieved ✓

## Next Steps

1. **Priority 1**: Analyze callback functions (FUN_08092988, FUN_08092990)
2. **Priority 2**: Analyze socket registration (FUN_0806efa4)
3. **Priority 3**: Investigate timestamp usage patterns
4. **Priority 4**: Trace pointer globals to their targets
5. **Priority 5**: Determine purpose of DAT_080de1f0 (zero init)

Once we understand these through analysis, we can properly name them.

