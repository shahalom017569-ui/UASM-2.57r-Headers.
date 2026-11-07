; math.inc - small inline integer and scalar-float helpers for UASM 2.57r
; Include windows.inc first only if you need its type names.
;
; These are assembler macros, not callable procedures. They emit instructions
; at the call site. Integer destination must be a 32-bit register. Float
; operands/destination are memory operands or labels; helpers clobber XMM0.
; Floating-point helpers require SSE (available on all Windows x64 targets).
;
; Signed integer division: MATH_I32_DIV quotient, remainder, dividend, divisor
; uses EAX/EDX and requires divisor not be EAX or EDX. Division by zero and
; signed overflow (INT_MIN / -1) must be handled by the caller.

IFNDEF UASM_MATH_INC_INCLUDED
UASM_MATH_INC_INCLUDED EQU 1

MATH_I32_ADD MACRO dst, lhs, rhs
    mov dst, lhs
    add dst, rhs
ENDM

MATH_I32_SUB MACRO dst, lhs, rhs
    mov dst, lhs
    sub dst, rhs
ENDM

MATH_I32_MUL MACRO dst, lhs, rhs
    imul dst, lhs, rhs
ENDM

MATH_I32_NEG MACRO dst, src
    mov dst, src
    neg dst
ENDM

MATH_I32_ABS MACRO dst, src
    mov dst, src
    test dst, dst
    jns @F
    neg dst
@@:
ENDM

MATH_I32_MIN MACRO dst, lhs, rhs
    mov dst, lhs
    cmp dst, rhs
    jle @F
    mov dst, rhs
@@:
ENDM

MATH_I32_MAX MACRO dst, lhs, rhs
    mov dst, lhs
    cmp dst, rhs
    jge @F
    mov dst, rhs
@@:
ENDM

MATH_I32_CLAMP MACRO dst, value, low, high
    mov dst, value
    cmp dst, low
    jge @F
    mov dst, low
    jmp @F2
@@:
    cmp dst, high
    jle @F2
    mov dst, high
@@F2:
ENDM

; Signed division. Outputs quotient and remainder to caller-selected registers.
MATH_I32_DIV MACRO quotient, remainder, dividend, divisor
    mov eax, dividend
    cdq
    idiv divisor
    mov quotient, eax
    mov remainder, edx
ENDM

; IEEE-754 single precision operations. dst/lhs/rhs are memory operands.
MATH_F32_ADD MACRO dst, lhs, rhs
    movss xmm0, lhs
    addss xmm0, rhs
    movss dst, xmm0
ENDM

MATH_F32_SUB MACRO dst, lhs, rhs
    movss xmm0, lhs
    subss xmm0, rhs
    movss dst, xmm0
ENDM

MATH_F32_MUL MACRO dst, lhs, rhs
    movss xmm0, lhs
    mulss xmm0, rhs
    movss dst, xmm0
ENDM

MATH_F32_DIV MACRO dst, lhs, rhs
    movss xmm0, lhs
    divss xmm0, rhs
    movss dst, xmm0
ENDM

MATH_F32_SQRT MACRO dst, src
    movss xmm0, src
    sqrtss xmm0, xmm0
    movss dst, xmm0
ENDM

ENDIF ; UASM_MATH_INC_INCLUDED
