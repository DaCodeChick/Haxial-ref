# Unknowns Documentation Checklist

## Purpose

This checklist ensures that unknowns are properly documented for future revisitation, without inventing placeholder names that hide gaps in understanding.

## When to Document

Document in `unknowns-to-revisit.md` when you have:
- Variables you don't understand yet: `param_1`, `uVar2`, `local_8c` (leave as-is)
- Struct offsets being accessed: `param_1[9]` → note offset +0x24
- Guessed names: `lock_or_refcount` (uncertainty)
- Gap fields: `gap_0x08[8]` (unknown struct regions)
- Function stubs: `FUN_xxxxxxxx` still being called

## What to Document

For each unknown, include:
1. **Location**: Function name and address, or struct name
2. **Variable/Offset**: The raw name (param_1, uVar2) or offset (+0x24)
3. **What you DON'T know**: Be explicit about gaps in understanding
4. **How it's used**: Observations from the code
5. **Questions**: Specific questions to investigate later
6. **Related unknowns**: Cross-reference other unknowns that might be connected
7. **Candidates**: Possible interpretations based on context

## Example Entry Format

```markdown
### function_name (0x12345678)
- **param_1[9]** (offset +0x24) - What is this?
  - Used: How the variable is used in the code
  - Question: Specific question about what it is
  - Candidates: Possible interpretations
  - Related: Other unknowns that might help explain this one
  - TODO: Next steps to investigate

- **uVar2** - What does this represent?
  - Used: Result of some_function(), compared with param_3
  - Question: Is this a count, offset, or something else?
```

## Red Flags (Must Document)

These patterns indicate unknowns that MUST be documented:

- ✗ Variables still named param_1, uVar2, local_8c without explanation
- ✗ Struct offsets accessed (param_1[9]) without knowing what's at that offset
- ✗ `gap_0xNN[size]` without candidates for what's in the gap
- ✗ Field names with "or" in them: `lock_or_refcount` (indicates uncertainty)
- ✗ Helper functions still named FUN_xxxxxxxx being called

## Good Examples (Clear What They Are)

These show understanding and don't need extra documentation:
- ✓ `socket_fd` - Clear what it is
- ✓ `bytes_sent` - Clear meaning
- ✓ `connection_magic` - Clear purpose
- ✓ `error_code` - Clear usage
- ✓ `buffer_size` - Clear meaning
- ✓ `packet_length` - Clear purpose

## Process Integration

During analysis workflow:

1. **Analyze** → See what a variable does
2. **Understand?** → 
   - YES: Rename it with specific name
   - NO: Leave it as-is (param_1, uVar2) and document
3. **Document** → Add entry to unknowns-to-revisit.md with offset/usage
4. **Continue** → Move to next item
5. **Review** → Periodically revisit unknowns and resolve them

## Don't Do This

Anti-patterns to avoid:
- ✗ Renaming param_1 to `temp1` (hides that you don't know what it is)
- ✗ Renaming uVar2 to `flags` (pretends you know it's flags)
- ✗ Creating struct field `field_0x08` (just note the offset in doc)
- ✗ Renaming local_8c to `result` (vague and uninformative)
- ✗ Moving on without documenting what you accessed

## Do This Instead

Good patterns:
- ✓ Leave param_1 as param_1, document: "param_1[9] - offset +0x24 - appears to be queue head"
- ✓ Leave uVar2 as uVar2, document: "uVar2 - return value of send(), meaning unclear"
- ✓ Note offsets: "param_1[9] accessed (offset +0x24), need to understand this field"
- ✓ For guesses, mark them: `lock_or_refcount` [GUESS: could be lock OR refcount]

## Periodic Review

Schedule review of unknowns-to-revisit.md:
- After analyzing 10 functions
- After completing a subsystem (e.g., all socket functions)
- When you encounter related unknowns
- Before documenting the protocol

## Success Criteria

The unknowns-to-revisit.md document is successful when:
- Every generic name has an entry
- Every gap in struct has candidates listed
- Questions are specific and actionable
- Cross-references help guide investigation
- Resolved items are marked as resolved with findings

## Anti-Patterns to Avoid

Don't do this:
- Using `temp1` without saying why it's temp or what it might be
- Using `flags` without investigating what flags
- Using `ptr` without knowing what it points to
- Creating struct gaps without any hypothesis about contents
- Naming something with "or" without plan to resolve ambiguity
- Moving on without documenting uncertainty

