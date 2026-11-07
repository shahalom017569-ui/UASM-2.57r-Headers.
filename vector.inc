; vector.inc - 2D/3D single-precision vector layout and inline helpers
; UASM 2.57r MASM-style syntax. Uses SSE; enable it for x86 builds.
; VEC2 is 8 bytes; VEC3 is 16 bytes with a padding lane.
; Pass base registers for vector pointers. Dot/length-squared results are in XMM0.
; Arithmetic macros clobber XMM0-XMM1. Inputs are unchanged; dst may alias an
; input for add/sub/scale.

IFNDEF UASM_VECTOR_INC_INCLUDED
UASM_VECTOR_INC_INCLUDED EQU 1

VEC2 STRUCT
    x REAL4 ?
    y REAL4 ?
VEC2 ENDS

VEC3 STRUCT
    x     REAL4 ?
    y     REAL4 ?
    z     REAL4 ?
    _pad  REAL4 ?
VEC3 ENDS

VEC2_ADD MACRO dst, lhs, rhs
    movlps xmm0, QWORD PTR [lhs]
    movlps xmm1, QWORD PTR [rhs]
    addps xmm0, xmm1
    movlps QWORD PTR [dst], xmm0
ENDM

VEC2_SUB MACRO dst, lhs, rhs
    movlps xmm0, QWORD PTR [lhs]
    movlps xmm1, QWORD PTR [rhs]
    subps xmm0, xmm1
    movlps QWORD PTR [dst], xmm0
ENDM

VEC2_SCALE MACRO dst, src, scalarMem
    movlps xmm0, QWORD PTR [src]
    movss xmm1, scalarMem
    shufps xmm1, xmm1, 0
    mulps xmm0, xmm1
    movlps QWORD PTR [dst], xmm0
ENDM

VEC2_DOT MACRO lhs, rhs
    movss xmm0, DWORD PTR [lhs+0]
    mulss xmm0, DWORD PTR [rhs+0]
    movss xmm1, DWORD PTR [lhs+4]
    mulss xmm1, DWORD PTR [rhs+4]
    addss xmm0, xmm1
ENDM

VEC3_ADD MACRO dst, lhs, rhs
    movups xmm0, XMMWORD PTR [lhs]
    movups xmm1, XMMWORD PTR [rhs]
    addps xmm0, xmm1
    movups XMMWORD PTR [dst], xmm0
ENDM

VEC3_SUB MACRO dst, lhs, rhs
    movups xmm0, XMMWORD PTR [lhs]
    movups xmm1, XMMWORD PTR [rhs]
    subps xmm0, xmm1
    movups XMMWORD PTR [dst], xmm0
ENDM

VEC3_SCALE MACRO dst, src, scalarMem
    movups xmm0, XMMWORD PTR [src]
    movss xmm1, scalarMem
    shufps xmm1, xmm1, 0
    mulps xmm0, xmm1
    movups XMMWORD PTR [dst], xmm0
ENDM

VEC3_DOT MACRO lhs, rhs
    movss xmm0, DWORD PTR [lhs+0]
    mulss xmm0, DWORD PTR [rhs+0]
    movss xmm1, DWORD PTR [lhs+4]
    mulss xmm1, DWORD PTR [rhs+4]
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [lhs+8]
    mulss xmm1, DWORD PTR [rhs+8]
    addss xmm0, xmm1
ENDM

VEC2_LENGTH_SQ MACRO src
    movss xmm0, DWORD PTR [src+0]
    mulss xmm0, xmm0
    movss xmm1, DWORD PTR [src+4]
    mulss xmm1, xmm1
    addss xmm0, xmm1
ENDM

VEC3_LENGTH_SQ MACRO src
    movss xmm0, DWORD PTR [src+0]
    mulss xmm0, xmm0
    movss xmm1, DWORD PTR [src+4]
    mulss xmm1, xmm1
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [src+8]
    mulss xmm1, xmm1
    addss xmm0, xmm1
ENDM

ENDIF ; UASM_VECTOR_INC_INCLUDED
