# Globals Renaming Review

## Globals Previously Renamed

Let me review each one to see if we actually knew what they were:

### From parse_args_and_setup_signals (0x08092198)

1. **DAT_080db6e2 → g_quiet_mode** ✓ KNOWN
   - Used: Set when --nohup flag found
   - Clear purpose: quiet mode flag

2. **DAT_080ddaa8 → g_timestamp_usec** ❌ GUESS
   - Used: Stores tv_usec from gettimeofday
   - Question: Why stored? What's it used for?

3. **DAT_080ddaa0 → g_timestamp_usec_copy** ❌ GUESS
   - Used: Another usec storage
   - Question: Why a "copy"? What's the relationship?

4. **DAT_080ddaac → g_timestamp_msec** ❌ GUESS
   - Used: Stores tv_sec from gettimeofday
   - Question: Why stored globally?

5. **DAT_080ddaa4 → g_timestamp_msec_copy** ❌ GUESS
   - Used: Another sec storage
   - Question: Why two copies of timestamps?

6. **DAT_080de1d4 → g_data_ptr** ❌ WRONG - Generic name
   - Used: Set to some pointer value
   - Unknown: What does it point to?

7. **DAT_080de1ec → g_data_ptr_2** ❌ WRONG - Generic name
   - Used: Another pointer
   - Unknown: What does it point to?

8. **DAT_080de1f0 → g_zero_init** ❌ WRONG - Generic name
   - Used: Initialized to 0
   - Unknown: What is it for?

9. **DAT_080db6e8 → g_callback_1** ❌ WRONG - Generic name
   - Used: Set to FUN_08092988 address
   - Unknown: What is FUN_08092988? Callback for what?

10. **DAT_080db6ec → g_callback_2** ❌ WRONG - Generic name
    - Used: Set to FUN_08092990 address
    - Unknown: What is FUN_08092990? Callback for what?

### From main (0x08048ddc)

11. **DAT_080db6e4 → g_cleanup_callback** ✓ KNOWN (or good guess)
    - Used: Called at program exit if non-null
    - Purpose: cleanup function pointer

### From create_socket (0x08099bd8)

12. **DAT_080ddb30 → g_socket_list_or_context** ❌ "or" indicates GUESS
    - Used: Passed to FUN_0806efa4
    - Question: Is it a list head? Context struct? Something else?

13. **DAT_080ddafc → g_sockets_initialized** ✓ KNOWN
    - Used: Boolean flag, checked before initialization
    - Clear purpose: init flag

### From allocate_memory (0x0808f6cc)

14. **DAT_080db6f8 → g_malloc_count** ✓ KNOWN
    - Used: Incremented on malloc, decremented on free
    - Clear purpose: allocation counter

## Summary

### Should Keep (Actually Known): 4
- g_quiet_mode ✓
- g_cleanup_callback ✓
- g_sockets_initialized ✓
- g_malloc_count ✓

### Should Revert (Generic/Unknown): 8
- g_timestamp_usec → DAT_080ddaa8 (document usage)
- g_timestamp_usec_copy → DAT_080ddaa0 (document usage)
- g_timestamp_msec → DAT_080ddaac (document usage)
- g_timestamp_msec_copy → DAT_080ddaa4 (document usage)
- g_data_ptr → DAT_080de1d4 (document what it points to)
- g_data_ptr_2 → DAT_080de1ec (document what it points to)
- g_zero_init → DAT_080de1f0 (document purpose)
- g_callback_1 → DAT_080db6e8 (document after analyzing FUN_08092988)
- g_callback_2 → DAT_080db6ec (document after analyzing FUN_08092990)

### Needs Better Name (Currently GUESS): 1
- g_socket_list_or_context → Keep as-is with [GUESS] or revert and document

## Recommended Actions

1. Revert the 8 generic/unknown names back to DAT_*
2. Document all of them in unknowns-to-revisit.md with usage patterns
3. Consider reverting g_socket_list_or_context since "or" indicates uncertainty
4. Keep the 4 that we genuinely understand

