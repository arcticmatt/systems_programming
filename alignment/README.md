# Alignment

## Overview

**Definition**: 

- A memory pointer that refers to primitive data that is `n` bytes long is said to be aligned if it is only allowed to contain addresses that are `n`-byte aligned, otherwise it is said to be unaligned.
    - E.g. `uint32_t a` is aligned if it is at address `24`
    - E.g. `uint8_t b` is always aligned
- A memory pointer that refers to a data aggregate (a data structure or array) is aligned if (and only if) each primitive datum in the aggregate is aligned.

## Resources

- [https://en.wikipedia.org/wiki/Data_structure_alignment](https://en.wikipedia.org/wiki/Data_structure_alignment)
- [https://developer.apple.com/documentation/code_diagnostics/undefined_behavior_sanitizer/misaligned_pointer](https://developer.apple.com/documentation/code_diagnostics/undefined_behavior_sanitizer/misaligned_pointer)
