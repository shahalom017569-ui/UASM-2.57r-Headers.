# UASM 2.57r Headers

Compact, modular MASM-style include files and examples for UASM 2.57r.

## Math and geometry modules

- `math.inc` — inline signed 32-bit integer and basic scalar `REAL4` helpers.
- `float.inc` — scalar `REAL4` arithmetic, min/max, abs/neg, lerp, conversions, comparison, and x87 sine/cosine macros.
- `vector.inc` — `VEC2`/`VEC3` layouts and common SSE vector operations.
- `matrix.inc` — row-major `MAT4`, identity/zero initialization, and 4x4 multiplication.
- `transform.inc` — 2D `MAT3` and 3D `MAT4` constructors, composition, point/vector transforms, and homogeneous projection.

The modules are inline macros (no runtime library required). SSE helpers require
SSE enabled for x86; SSE2 is guaranteed on Windows x64. `float.inc` sine/cosine
use x87 `FSIN`/`FCOS`; reduce large angles and check the x87 C2 status if range
handling matters. Matrix multiplication uses x87 and leaves the control word
unchanged. Transform matrices are row-major and multiply column vectors; matrix
composition order therefore follows `result = lhs * rhs`.

Transform constructors take sine/cosine values from the caller rather than
silently approximating trigonometry. Review each include's comments for operand
forms, register clobbers, alignment, aliasing, and in-place operation rules.

## Windows API include

`windows.inc` contains selected types, constants, structures, and prototypes for
Kernel32, User32, GDI32, Advapi32, and Shell32. Set `WINDOWS_X64 EQU 1` before
including it for a 64-bit target; otherwise it defaults to x86. Set
`WINDOWS_AUTOLIB EQU 1` to emit import-library directives, or add only the
`INCLUDELIB` directives your program needs.

## Examples

- `math_vectors.asm` demonstrates integer/scalar math, vectors, and matrices.
- `transform_pipeline.asm` demonstrates scalar float operations and 2D/3D transforms.

These examples are source illustrations; assemble with UASM 2.57r for the target
architecture and link the appropriate Windows libraries when using Windows APIs.
