# UASM 2.57r Headers

Compact, modular MASM-style include files and examples for UASM 2.57r.

## Math and geometry modules

- `math.inc` — inline signed integer and scalar helpers.
- `float.inc` — scalar REAL4 arithmetic, conversions, comparisons, and trigonometry.
- `vector.inc` — VEC2/VEC3 layouts and common SSE vector operations.
- `matrix.inc` — row-major 4x4 matrices and multiplication.
- `transform.inc` — 2D affine and 3D homogeneous transform helpers.
- `statistics.inc` — basic statistical helpers.
- `complex.inc` — complex arithmetic and magnitude.
- `quaternion.inc` — quaternion identity, conjugate, multiplication, normalization, and matrix conversion.
- `camera.inc` — right-handed view plus perspective and orthographic projection matrices.

These are assembler include files with inline macros, not a complete SDK or a
substitute for a tested numerical library. Review each module's comments for
register usage, input assumptions, alias restrictions, and matrix conventions.

## Matrix and coordinate conventions

Matrices are row-major in memory and multiply column vectors: `result = M * v`.
The camera helpers use a right-handed view looking down -Z and OpenGL-style NDC
Z in [-1,1]. Perspective parameters use precomputed x/y scales; the caller is
responsible for computing `cot(fovY/2)` and any sine/cosine values.

## Windows API include

`windows.inc` contains selected Win32/Win64 types, constants, structures, and
prototypes for Kernel32, User32, GDI32, Advapi32, and Shell32. Set
`WINDOWS_X64 EQU 1` before including it for a 64-bit target; otherwise it
defaults to x86. `WINDOWS_AUTOLIB EQU 1` emits import-library directives.

## Examples

- `math_vectors.asm`
- `transform_pipeline.asm`
- `messagebox.asm`
- `window.asm`
