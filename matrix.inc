; matrix.inc - 4x4 single-precision matrix layout and basic helpers
; Matrices are row-major: element (row, column) is at row*16 + column*4.
; Matrix routines use unaligned memory accesses and do not require alignment.
; Include this in a source that supports SSE; the macros clobber XMM0.

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

; dst is a base register pointing to writable 64-byte MAT4 storage.
MAT4_ZERO MACRO dst
    xor eax, eax
    mov DWORD PTR [dst+0], eax
    mov DWORD PTR [dst+4], eax
    mov DWORD PTR [dst+8], eax
    mov DWORD PTR [dst+12], eax
    mov DWORD PTR [dst+16], eax
    mov DWORD PTR [dst+20], eax
    mov DWORD PTR [dst+24], eax
    mov DWORD PTR [dst+28], eax
    mov DWORD PTR [dst+32], eax
    mov DWORD PTR [dst+36], eax
    mov DWORD PTR [dst+40], eax
    mov DWORD PTR [dst+44], eax
    mov DWORD PTR [dst+48], eax
    mov DWORD PTR [dst+52], eax
    mov DWORD PTR [dst+56], eax
    mov DWORD PTR [dst+60], eax
ENDM

MAT4_IDENTITY MACRO dst
    mov DWORD PTR [dst+0], 03F800000h
    mov DWORD PTR [dst+4], 0
    mov DWORD PTR [dst+8], 0
    mov DWORD PTR [dst+12], 0
    mov DWORD PTR [dst+16], 0
    mov DWORD PTR [dst+20], 03F800000h
    mov DWORD PTR [dst+24], 0
    mov DWORD PTR [dst+28], 0
    mov DWORD PTR [dst+32], 0
    mov DWORD PTR [dst+36], 0
    mov DWORD PTR [dst+40], 03F800000h
    mov DWORD PTR [dst+44], 0
    mov DWORD PTR [dst+48], 0
    mov DWORD PTR [dst+52], 0
    mov DWORD PTR [dst+56], 0
    mov DWORD PTR [dst+60], 03F800000h
ENDM

; Store a 4x4 matrix product into dst using the scalar x87 FPU.
; Arguments are base registers. dst must not overlap lhs or rhs. This macro
; emits a fully unrolled multiply and does not change the x87 control word.
MAT4_MUL MACRO dst, lhs, rhs
    LOCAL row, col
    ; Expand one dot product per output cell; EAX/ECX are scratch registers.
    mov eax, 0
    REPT 4
        mov ecx, 0
        REPT 4
            fld DWORD PTR [lhs+eax+0]
            fmul DWORD PTR [rhs+ecx+0]
            fld DWORD PTR [lhs+eax+4]
            fmul DWORD PTR [rhs+ecx+16]
            faddp st(1), st(0)
            fld DWORD PTR [lhs+eax+8]
            fmul DWORD PTR [rhs+ecx+32]
            faddp st(1), st(0)
            fld DWORD PTR [lhs+eax+12]
            fmul DWORD PTR [rhs+ecx+48]
            faddp st(1), st(0)
            fstp DWORD PTR [dst+eax+ecx*4]
            add ecx, 1
        ENDM
        add eax, 16
    ENDM
ENDM

ENDIF ; UASM_MATRIX_INC_INCLUDED
