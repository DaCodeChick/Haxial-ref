# AGENTS.md Updates - Session 2

## Summary

Enhanced AGENTS.md with critical guidance on handling unknowns and naming philosophy to prevent false confidence and ensure documented gaps in understanding.

## New Sections Added

### 1. NAMING PHILOSOPHY (after Ghidra-Specific Tips)

**Purpose:** Establish principle of "clarity and honesty" over perfection

**Key Points:**
- Known variables get specific names
- Partially known variables get descriptive names with [GUESS] markers
- Unknown variables get generic names BUT must be documented immediately
- Rule: If you use a generic name, you MUST document it in unknowns-to-revisit.md

**Why Added:**
- Prevents false confidence from using specific-sounding names for unknowns
- Makes explicit that documentation is required for generic names
- Establishes expectation: generic names = temporary + documented

### 2. CRITICAL: DOCUMENTING UNKNOWNS (after ZERO TOLERANCE section)

**Purpose:** Provide concrete process for handling unknowns

**Key Points:**
- Document immediately in unknowns-to-revisit.md
- Include: function/location, name chosen, what you DON'T know, how it's used, questions, cross-refs
- Examples of good documentation entries
- Examples of what NOT to do
- Struct gap documentation patterns

**Why Added:**
- Previous section said "use generic names" but didn't require documentation
- Need explicit process for handling uncertainty
- Examples make it concrete and actionable

### 3. Enhanced TYPE CORRECTION WORKFLOW

**Added Step 5:** Document any unknowns/assumptions in unknowns-to-revisit.md

**Why Added:**
- Workflow was missing the critical documentation step
- Ensures documentation happens as part of normal workflow

### 4. Enhanced STRUCT CREATION WORKFLOW

**Updated Example:**
- Added comments showing KNOWN vs GUESS vs UNKNOWN
- Shows how to mark assumptions inline

**Why Added:**
- Example now demonstrates proper uncertainty marking
- Shows integration with unknowns documentation

## Documentation Philosophy Established

### Three-Tier Naming System

1. **KNOWN** - Verified, specific names
   - Example: `socket_fd` (we know it's the file descriptor)
   
2. **GUESS** - Descriptive but uncertain, marked with [GUESS]
   - Example: `lock_or_refcount [GUESS: could be lock OR refcount]`
   
3. **UNKNOWN** - Generic names, immediately documented
   - Example: `temp1` → documented in unknowns-to-revisit.md

### Uncertainty Markers

- Inline comments: `// +0x04 - GUESS: lock or refcount?`
- Field names with "or": `lock_or_refcount`
- Gap fields: `gap_0x08[8]` with TODO comments
- Generic names: Requires documentation

## Anti-Patterns Now Explicitly Forbidden

### Don't Do This:
- ✗ Use `temp1` without documenting what you don't know
- ✗ Use `flags` without investigating which flags
- ✗ Use `ptr` without knowing what it points to
- ✗ Create struct gaps without hypothesis about contents
- ✗ Move on without documenting uncertainty

### Do This Instead:
- ✓ Use `temp1` AND immediately document: "temp1 - UNKNOWN: purpose unclear, appears to be..."
- ✓ Use `flags` AND document: "flags - UNKNOWN: which flags? Need to check fcntl.h"
- ✓ Use `ptr` AND document: "ptr - GUESS: likely points to queue entry structure"
- ✓ Create `gap_0x08[8]` AND document: "gap_0x08[8] - UNKNOWN: candidates include..."
- ✓ Document before moving on

## Integration Points

The unknowns documentation requirement is now integrated into:

1. **NAMING PHILOSOPHY** - Establishes the principle
2. **ZERO TOLERANCE section** - Specifies generic names are acceptable if documented
3. **CRITICAL: DOCUMENTING UNKNOWNS** - Provides the process
4. **TYPE CORRECTION WORKFLOW** - Adds documentation step
5. **STRUCT CREATION WORKFLOW** - Shows how to mark unknowns in structs

## Expected Impact

### Positive Outcomes:
- No more false confidence from specific-sounding names
- Clear record of what's unknown for future investigation
- Systematic approach to resolving uncertainties
- Better understanding of analysis gaps

### Behavioral Changes:
- Agent will document every generic name used
- Agent will mark guesses explicitly with [GUESS]
- Agent will update unknowns document as part of normal workflow
- Agent will cross-reference related unknowns

## Verification

To verify compliance, check:
1. Are generic names (temp*, flags, result, ptr) documented in unknowns-to-revisit.md?
2. Are struct gaps documented with candidates?
3. Are guesses marked with [GUESS] or "or" in the name?
4. Are questions specific and actionable?
5. Are cross-references provided for related unknowns?

## Next Steps

1. Review existing renamed variables for undocumented unknowns
2. Update unknowns-to-revisit.md with all generic names used
3. Revisit generic names as understanding improves
4. Keep unknowns document updated throughout analysis

