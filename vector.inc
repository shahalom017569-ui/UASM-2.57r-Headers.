; vector.inc - 2D/3D single-precision vector layout and inline helpers
; UASM 2.57r, MASM-style syntax. Include math.inc if scalar helpers are needed.
;
; Vec2 is exactly two REAL4 values (8 bytes).
; Vec3 has a fourth padding float (16 bytes), allowing safe 128-bit loads.
; The helpers use SSE and clobber XMM0-XMM3 as noted. Vector pointers are
; passed as base registers (for example ESI/EDI in x86 or RSI/RDI in x64).
; VEC2_ADD/ SUB/ SCALE use XMM0. VEC3_ADD/SUB/SCALE use XMM0.
; Dot products return their scalar result in XMM0. Inputs are unchanged.

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

; dst, lhs, rhs are base registers pointing to VEC2 values.
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
    shufps xmm1, xmm0, 0
    movss xmm1, scalarMem
    shufps xmm1, xmm1, 0
    mulps xmm0, xmm1
    movlps QWORD PTR [dst], xmm0
ENDM

; Returns x*x + y*y in the low lane of XMM0.
VEC2_DOT MACRO lhs, rhs
    movlps xmm0, QWORD PTR [lhs]
    movlps xmm1, QWORD PTR [rhs]
    mulps xmm0, xmm1
    movaps xmm1, xmm0
    shufps xmm1, xmm1, 0B1h
    addss xmm0, xmm1
ENDM

; dst/lhs/rhs are base registers pointing to 16-byte VEC3 values.
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

; Returns x*x + y*y + z*z in the low lane of XMM0.
VEC3_DOT MACRO lhs, rhs
    movups xmm0, XMMWORD PTR [lhs]
    movups xmm1, XMMWORD PTR [rhs]
    mulps xmm0, xmm1
    movaps xmm1, xmm0
    shufps xmm1, xmm1, 4Eh
    addss xmm0, xmm1
    movaps xmm1, xmm0
    shufps xmm1, xmm1, 0B1h
    addss xmm0, xmm1
ENDM

; VEC2/VEC3 length squared helpers leave the result in XMM0.
VEC2_LENGTH_SQ MACRO src
    movlps xmm0, QWORD PTR [src]
    mulps xmm0, xmm0
    movaps xmm1, xmm0
    shufps xmm1, xmm1, 0B1h
    addss xmm0, xmm1
ENDM

VEC3_LENGTH_SQ MACRO src
    movups xmm0, XMMWORD PTR [src]
    mulps xmm0, xmm0
    movaps xmm1, xmm0
    shufps xmm1, xmm1, 4Eh
    addss xmm0, xmm1
    movaps xmm1, xmm0
    shufps xmm1, xmm1, 0B1h
    addss xmm0, xmm1
ENDM

ENDIF ; UASM_VECTOR_INC_INCLUDED
