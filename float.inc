; float.inc - scalar IEEE-754 REAL4 helpers for UASM 2.57r
; Inline macros; no runtime library required.
;
; Memory operands are used for scalar values unless a macro says otherwise.
; Arithmetic macros clobber XMM0; F32_LERP also uses XMM1/XMM2.
; F32_ABS/NEG clobber EAX and XMM0-XMM1. Conversions clobber XMM0.
; F32_TO_I32 truncates toward zero (CVTTSS2SI). F32_SIN/COS use x87 and
; require finite input in the FSIN/FCOS supported range; reduce angles first.
;
; SSE is required (SSE2 is guaranteed by Windows x64; enable SSE for x86).

IFNDEF UASM_FLOAT_INC_INCLUDED
UASM_FLOAT_INC_INCLUDED EQU 1

F32_ADD MACRO dst, lhs, rhs
    movss xmm0, lhs
    addss xmm0, rhs
    movss dst, xmm0
ENDM

F32_SUB MACRO dst, lhs, rhs
    movss xmm0, lhs
    subss xmm0, rhs
    movss dst, xmm0
ENDM

F32_MUL MACRO dst, lhs, rhs
    movss xmm0, lhs
    mulss xmm0, rhs
    movss dst, xmm0
ENDM

F32_DIV MACRO dst, lhs, rhs
    movss xmm0, lhs
    divss xmm0, rhs
    movss dst, xmm0
ENDM

F32_SQRT MACRO dst, src
    movss xmm0, src
    sqrtss xmm0, xmm0
    movss dst, xmm0
ENDM

; MINSS/MAXSS follow x86 SSE NaN and signed-zero behavior.
F32_MIN MACRO dst, lhs, rhs
    movss xmm0, lhs
    minss xmm0, rhs
    movss dst, xmm0
ENDM

F32_MAX MACRO dst, lhs, rhs
    movss xmm0, lhs
    maxss xmm0, rhs
    movss dst, xmm0
ENDM

F32_ABS MACRO dst, src
    movss xmm0, src
    movd eax, xmm0
    and eax, 07FFFFFFFh
    movd xmm0, eax
    movss dst, xmm0
ENDM

F32_NEG MACRO dst, src
    movss xmm0, src
    movd eax, xmm0
    xor eax, 080000000h
    movd xmm0, eax
    movss dst, xmm0
ENDM

; dst = a + (b-a)*t. All operands are REAL4 memory operands.
F32_LERP MACRO dst, a, b, t
    movss xmm0, b
    subss xmm0, a
    mulss xmm0, t
    addss xmm0, a
    movss dst, xmm0
ENDM

; Signed integer conversions. F32_TO_I32 truncates toward zero.
I32_TO_F32 MACRO dst, src
    cvtsi2ss xmm0, src
    movss dst, xmm0
ENDM

F32_TO_I32 MACRO dst, src
    movss xmm0, src
    cvttss2si dst, xmm0
ENDM

; Compare src1 with src2; leaves EFLAGS as set by UCOMISS.
; JP detects unordered (NaN), JA means src1 > src2, JB means src1 < src2.
F32_COMPARE MACRO src1, src2
    movss xmm0, src1
    ucomiss xmm0, src2
ENDM

; x87 trigonometric instructions. Input and output are REAL4 memory operands.
; FSIN/FCOS set C2 when the argument is outside their supported range.
F32_SIN MACRO dst, src
    fld src
    fsin
    fstp dst
ENDM

F32_COS MACRO dst, src
    fld src
    fcos
    fstp dst
ENDM

ENDIF ; UASM_FLOAT_INC_INCLUDED
