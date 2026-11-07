; statistics.inc - basic statistical helpers for REAL4 data
; Depends on float.inc. These macros operate on REAL4 memory operands and
; clobber XMM0-XMM2. Use labels, locals, or memory addresses only.

INCLUDE float.inc

IFNDEF UASM_STATISTICS_INC_INCLUDED
UASM_STATISTICS_INC_INCLUDED EQU 1

; Compute mean of three REAL4 values: (a + b + c) / 3.
STAT_MEAN3 MACRO dst, a, b, c
    movss xmm0, a
    addss xmm0, b
    addss xmm0, c
    movss xmm1, THREE_F32
    divss xmm0, xmm1
    movss dst, xmm0
ENDM

; Compute max of three REAL4 values; if a > b and a > c, result is a, etc.
STAT_MAX3 MACRO dst, a, b, c
    movss xmm0, a
    maxss xmm0, b
    maxss xmm0, c
    movss dst, xmm0
ENDM

STAT_MIN3 MACRO dst, a, b, c
    movss xmm0, a
    minss xmm0, b
    minss xmm0, c
    movss dst, xmm0
ENDM

; Compute variance of three values: ((a-m)^2 + (b-m)^2 + (c-m)^2) / 3
STAT_VARIANCE3 MACRO dst, a, b, c
    LOCAL meanValue
    DATASEG
    meanValue REAL4 0.0
    CODESEG
    STAT_MEAN3 meanValue, a, b, c
    movss xmm0, a
    subss xmm0, meanValue
    mulss xmm0, xmm0
    movss xmm1, b
    subss xmm1, meanValue
    mulss xmm1, xmm1
    addss xmm0, xmm1
    movss xmm1, c
    subss xmm1, meanValue
    mulss xmm1, xmm1
    addss xmm0, xmm1
    movss xmm1, THREE_F32
    divss xmm0, xmm1
    movss dst, xmm0
ENDM

; Standard deviation = sqrt(variance).
STAT_STDDEV3 MACRO dst, a, b, c
    LOCAL varValue
    DATASEG
    varValue REAL4 0.0
    CODESEG
    STAT_VARIANCE3 varValue, a, b, c
    F32_SQRT dst, varValue
ENDM

; Linear interpolation: result = a + (b - a) * t
STAT_LERP_F32 MACRO dst, a, b, t
    F32_LERP dst, a, b, t
ENDM

; Simple clamp to [low, high]
STAT_CLAMP_F32 MACRO dst, value, low, high
    movss xmm0, value
    minss xmm0, high
    maxss xmm0, low
    movss dst, xmm0
ENDM

; Constant used by mean/variance helpers.
THREE_F32 REAL4 3.0

ENDIF ; UASM_STATISTICS_INC_INCLUDED
