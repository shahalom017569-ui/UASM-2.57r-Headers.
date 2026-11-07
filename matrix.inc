; matrix.inc - row-major 4x4 single-precision matrix helpers
; Elements use row*16 + column*4 byte offsets. Matrix operations use x87
; scalar arithmetic and unaligned data; no alignment is required.
; MAT4_MUL computes dst = lhs * rhs. dst must not overlap either input.
; The x87 stack is balanced; the control word is unchanged.

IFNDEF UASM_MATRIX_INC_INCLUDED
UASM_MATRIX_INC_INCLUDED EQU 1

MAT4 STRUCT
    m00 REAL4 ?
    m01 REAL4 ?
    m02 REAL4 ?
    m03 REAL4 ?
    m10 REAL4 ?
    m11 REAL4 ?
    m12 REAL4 ?
    m13 REAL4 ?
    m20 REAL4 ?
    m21 REAL4 ?
    m22 REAL4 ?
    m23 REAL4 ?
    m30 REAL4 ?
    m31 REAL4 ?
    m32 REAL4 ?
    m33 REAL4 ?
MAT4 ENDS

MAT4_ZERO MACRO dst
    mov DWORD PTR [dst+0], 0
    mov DWORD PTR [dst+4], 0
    mov DWORD PTR [dst+8], 0
    mov DWORD PTR [dst+12], 0
    mov DWORD PTR [dst+16], 0
    mov DWORD PTR [dst+20], 0
    mov DWORD PTR [dst+24], 0
    mov DWORD PTR [dst+28], 0
    mov DWORD PTR [dst+32], 0
    mov DWORD PTR [dst+36], 0
    mov DWORD PTR [dst+40], 0
    mov DWORD PTR [dst+44], 0
    mov DWORD PTR [dst+48], 0
    mov DWORD PTR [dst+52], 0
    mov DWORD PTR [dst+56], 0
    mov DWORD PTR [dst+60], 0
ENDM

MAT4_IDENTITY MACRO dst
    MAT4_ZERO dst
    mov DWORD PTR [dst+0], 03F800000h
    mov DWORD PTR [dst+20], 03F800000h
    mov DWORD PTR [dst+40], 03F800000h
    mov DWORD PTR [dst+60], 03F800000h
ENDM

MAT4_MUL_CELL MACRO dst, lhs, rhs, outOff, rowOff, colOff
    fld DWORD PTR [lhs+rowOff+0]
    fmul DWORD PTR [rhs+colOff+0]
    fld DWORD PTR [lhs+rowOff+4]
    fmul DWORD PTR [rhs+colOff+16]
    faddp st(1), st(0)
    fld DWORD PTR [lhs+rowOff+8]
    fmul DWORD PTR [rhs+colOff+32]
    faddp st(1), st(0)
    fld DWORD PTR [lhs+rowOff+12]
    fmul DWORD PTR [rhs+colOff+48]
    faddp st(1), st(0)
    fstp DWORD PTR [dst+outOff]
ENDM

MAT4_MUL MACRO dst, lhs, rhs
    MAT4_MUL_CELL dst, lhs, rhs, 0, 0, 0
    MAT4_MUL_CELL dst, lhs, rhs, 4, 0, 4
    MAT4_MUL_CELL dst, lhs, rhs, 8, 0, 8
    MAT4_MUL_CELL dst, lhs, rhs, 12, 0, 12
    MAT4_MUL_CELL dst, lhs, rhs, 16, 16, 0
    MAT4_MUL_CELL dst, lhs, rhs, 20, 16, 4
    MAT4_MUL_CELL dst, lhs, rhs, 24, 16, 8
    MAT4_MUL_CELL dst, lhs, rhs, 28, 16, 12
    MAT4_MUL_CELL dst, lhs, rhs, 32, 32, 0
    MAT4_MUL_CELL dst, lhs, rhs, 36, 32, 4
    MAT4_MUL_CELL dst, lhs, rhs, 40, 32, 8
    MAT4_MUL_CELL dst, lhs, rhs, 44, 32, 12
    MAT4_MUL_CELL dst, lhs, rhs, 48, 48, 0
    MAT4_MUL_CELL dst, lhs, rhs, 52, 48, 4
    MAT4_MUL_CELL dst, lhs, rhs, 56, 48, 8
    MAT4_MUL_CELL dst, lhs, rhs, 60, 48, 12
ENDM

ENDIF ; UASM_MATRIX_INC_INCLUDED
