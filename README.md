# UASM 2.57r Headers

Compact, modular MASM-style include files and examples for UASM 2.57r.

## Math and geometry modules

- `math.inc` — inline signed 32-bit integer helpers and scalar `REAL4` SSE helpers.
- `vector.inc` — `VEC2`/`VEC3` layouts, add/subtract/scale, dot products, and squared lengths.
- `matrix.inc` — row-major `MAT4`, zero/identity initialization, and 4x4 multiplication.

These are inline macros (no runtime library required). `math.inc` floating-point
and vector helpers use SSE; `matrix.inc` multiplication uses x87. On x86, assemble
with an SSE-capable target for the SSE macros. On x64, SSE2 is part of the platform.
The macros document their registers and pointer/aliasing assumptions in each file.

## Windows API include

`windows.inc` contains selected types, constants, structures, and prototypes for
Kernel32, User32, GDI32, Advapi32, and Shell32. Set `WINDOWS_X64 EQU 1` before
including it for a 64-bit target; otherwise it defaults to x86. Set
`WINDOWS_AUTOLIB EQU 1` to emit import-library directives, or add only the
`INCLUDELIB` directives your program needs.

## Examples

`math_vectors.asm` demonstrates the math, vector, and matrix macros. Check each
module's comments for operand forms, clobbers, and in-place operation limits.
