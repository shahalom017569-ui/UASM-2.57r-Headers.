# UASM 2.57r Headers

Compact, modular MASM-style include files and examples for UASM 2.57r.

## Modules

- `math.inc` — inline signed integer helpers and basic scalar REAL4 operations.
- `float.inc` — scalar floating-point arithmetic, conversions, comparisons, and trigonometry.
- `vector.inc` — VEC2/VEC3 layouts and vector operations.
- `matrix.inc` — row-major 4x4 matrices and multiplication.
- `transform.inc` — 2D affine and 3D homogeneous transforms.
- `camera.inc` — right-handed view, perspective, and orthographic matrices.
- `quaternion.inc` — quaternion multiplication, normalization, and matrix conversion.
- `statistics.inc` — REAL4 array sum, mean, RMS, min/max, and population variance.
- `complex.inc` — complex arithmetic helpers.
- `windows.inc` — selected Win32/Win64 declarations and API prototypes.

## Math conventions and requirements

Matrices are row-major in memory and multiply column vectors (`result = M * v`).
The camera helpers use a right-handed coordinate system looking down -Z and
OpenGL-style NDC depth in [-1,+1]. The perspective helper takes precomputed
x/y scale values; it does not calculate the field-of-view trigonometry.

The math macros are inline assembly, not callable procedures. Most floating-point
and vector helpers require SSE/SSE2 enabled for x86 targets. Review each include
for register clobbers, input assumptions, destination aliasing, and empty-input
behavior. The statistics macros accept a count in an immediate or 32-bit register;
empty arrays return zero.

## Examples

- `math_vectors.asm` — integer/scalar math, vectors, and matrices.
- `transform_pipeline.asm` — composing 2D and 3D transforms.
- `camera_pipeline.asm` — composing model, view, and perspective matrices, then transforming a point to clip/NDC coordinates.
- `messagebox.asm` and `window.asm` — Windows API examples.

The examples are source illustrations. Assemble with UASM 2.57r for the intended
target and link the appropriate Windows libraries if the program uses Win32 APIs.
