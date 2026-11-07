; statistics.inc - basic REAL4 array statistics for UASM 2.57r
; SSE2 inline macros; no external runtime dependencies.
;
; ptr is a base register pointing to an array of REAL4 values. count must be a
; nonnegative immediate or 32-bit register value. Destinations are REAL4 memory
; operands. Empty arrays produce zero for all outputs. The macros clobber EAX,
; ECX, and XMM0-XMM2. Arrays are read-only; results are stored in destinations.

IFNDEF UASM_STATISTICS_INC_INCLUDED
UASM_STATISTICS_INC_INCLUDED EQU 1

; dst = sum(values[0..count-1])
STAT_SUM_REAL4 MACRO dst, ptr, count
    LOCAL sum_loop, sum_done
    mov ecx, count
    xor eax, eax
    xorps xmm0, xmm0
    test ecx, ecx
    jz sum_done
sum_loop:
    addss xmm0, DWORD PTR [ptr+eax*4]
    inc eax
    dec ecx
    jnz sum_loop
sum_done:
    movss DWORD PTR [dst], xmm0
ENDM

; dst = arithmetic mean. Empty input yields 0.0.
STAT_MEAN_REAL4 MACRO dst, ptr, count
    LOCAL mean_loop, mean_done
    mov ecx, count
    xor eax, eax
    xorps xmm0, xmm0
    test ecx, ecx
    jz mean_done
mean_loop:
    addss xmm0, DWORD PTR [ptr+eax*4]
    inc eax
    dec ecx
    jnz mean_loop
    cvtsi2ss xmm1, DWORD PTR [count]
    divss xmm0, xmm1
mean_done:
    movss DWORD PTR [dst], xmm0
ENDM

; dst = sqrt(sum(x*x)/count). Empty input yields 0.0.
STAT_RMS_REAL4 MACRO dst, ptr, count
    LOCAL rms_loop, rms_done
    mov ecx, count
    xor eax, eax
    xorps xmm0, xmm0
    test ecx, ecx
    jz rms_done
rms_loop:
    movss xmm1, DWORD PTR [ptr+eax*4]
    mulss xmm1, xmm1
    addss xmm0, xmm1
    inc eax
    dec ecx
    jnz rms_loop
    cvtsi2ss xmm1, DWORD PTR [count]
    divss xmm0, xmm1
    sqrtss xmm0, xmm0
rms_done:
    movss DWORD PTR [dst], xmm0
ENDM

; Store min and max. Empty input yields 0.0 for both outputs.
STAT_MIN_MAX_REAL4 MACRO dstMin, dstMax, ptr, count
    LOCAL minmax_loop, minmax_done, minmax_empty
    mov ecx, count
    test ecx, ecx
    jz minmax_empty
    xor eax, eax
    movss xmm0, DWORD PTR [ptr]
    movaps xmm1, xmm0
    inc eax
    dec ecx
    jz minmax_done
minmax_loop:
    movss xmm2, DWORD PTR [ptr+eax*4]
    minss xmm0, xmm2
    maxss xmm1, xmm2
    inc eax
    dec ecx
    jnz minmax_loop
    jmp minmax_done
minmax_empty:
    xorps xmm0, xmm0
    xorps xmm1, xmm1
minmax_done:
    movss DWORD PTR [dstMin], xmm0
    movss DWORD PTR [dstMax], xmm1
ENDM

; Population variance: sum((x-mean)^2)/count. meanPtr points to REAL4.
; Empty input yields 0.0.
STAT_VARIANCE_REAL4 MACRO dst, ptr, count, meanPtr
    LOCAL variance_loop, variance_done
    mov ecx, count
    xor eax, eax
    xorps xmm0, xmm0
    test ecx, ecx
    jz variance_done
variance_loop:
    movss xmm1, DWORD PTR [ptr+eax*4]
    subss xmm1, DWORD PTR [meanPtr]
    mulss xmm1, xmm1
    addss xmm0, xmm1
    inc eax
    dec ecx
    jnz variance_loop
    cvtsi2ss xmm1, DWORD PTR [count]
    divss xmm0, xmm1
variance_done:
    movss DWORD PTR [dst], xmm0
ENDM

ENDIF ; UASM_STATISTICS_INC_INCLUDED
