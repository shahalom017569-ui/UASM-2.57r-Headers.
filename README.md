# UASM-2.57r-Headers.

Headers for UASM 2.57r assembler.

## Included modules

- `windows.inc` — compact Win32/Win64 declarations, structures, and APIs.
- `math.inc` — integer arithmetic and scalar REAL4 helpers.
- `float.inc` — scalar REAL4 arithmetic, clamp, compare, lerp, conversion, sine, and cosine macros.
- `vector.inc` — `VEC2` and `VEC3` layouts plus common SSE vector operations.
- `matrix.inc` — row-major 4x4 matrix helpers and matrix multiplication.
- `transform.inc` — 2D affine and 3D homogeneous transform macros.
- `statistics.inc` — mean, variance, stddev, min/max, and interpolation helpers.
- `complex.inc` — complex-number arithmetic and magnitude macros.

## Design goals

The library is intentionally modular:

- small files with focused responsibilities
- macros that map directly to instructions
- no hidden runtime dependencies
- straightforward extension for scientific, numerical, and graphics work

## Notes

- SSE is required for all vector/REAL4 macros on x86 targets.
- Windows x64 already has SSE2 and is the recommended path for floating-point work.
- Trig helpers are direct asm macros; use caller-side angle reduction when needed.
- Matrix conventions are documented in each file, especially the row-major and column-vector behavior.

## Example programs

- `messagebox.asm`
- `window.asm`
- `math_vectors.asm`
- `transform_pipeline.asm`

These files illustrate the progression from basic Windows calls to scientific and graphics-style math use.
