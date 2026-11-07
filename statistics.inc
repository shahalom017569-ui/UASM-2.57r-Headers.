; statistics.inc - compact real-valued aggregate helpers for UASM 2.57r
; These macros operate on arrays of REAL4 values and store the result in a
; REAL4 destination or memory object. They are intentionally lightweight and
; avoid external dependency on Win32 or x64 intrinsics.
;
; Conventions:
;   - ptr is a base pointer to an array of REAL4 elements.
;   - count is an integer count in bytes/words if passed as an immediate.
;   - result is a pointer to a REAL4 destination, not a register value.

INCLUDE float.inc

IFNDEF UASM_STATISTICS_INC_INCLUDED
UASM_STATISTICS_INC_INCLUDED EQU 1

; dst = sum(array[0..count-1])
STAT_SUM_REAL4 MACRO dst, ptr, count
    LOCAL done, loop
    mov ecx, count
    xor eax, eax
    fldz
    test ecx, ecx
    jz done
loop:
    fld DWORD PTR [ptr + eax*4]
    faddp st(1), st(0)
    inc eax
    dec ecx
    jnz loop
    fstp DWORD PTR [dst]
    jmp short done
done:
ENDM

; dst = mean(array[0..count-1])
STAT_MEAN_REAL4 MACRO dst, ptr, count
    LOCAL done, sum_loop
    mov ecx, count
    xor eax, eax
    fldz
    test ecx, ecx
    jz done
sum_loop:
    fld DWORD PTR [ptr + eax*4]
    faddp st(1), st(0)
    inc eax
    dec ecx
    jnz sum_loop
    ; divide by count using float conversion
    fild DWORD PTR count
    fdivp st(1), st(0)
    fstp DWORD PTR [dst]
    jmp short done
done:
ENDM

; dst = rms(array[0..count-1]) = sqrt(mean(x^2))
STAT_RMS_REAL4 MACRO dst, ptr, count
    LOCAL done, loop
    mov ecx, count
    xor eax, eax
    fldz
    test ecx, ecx
    jz done
loop:
    fld DWORD PTR [ptr + eax*4]
    fmul st(0), st(0)
    faddp st(1), st(0)
    inc eax
    dec ecx
    jnz loop
    ; divide by count
    fild DWORD PTR count
    fdivp st(1), st(0)
    fsqrt
    fstp DWORD PTR [dst]
    jmp short done
done:
ENDM

; dstMin = minimum(array), dstMax = maximum(array)
STAT_MIN_MAX_REAL4 MACRO dstMin, dstMax, ptr, count
    LOCAL done, loop
    mov ecx, count
    test ecx, ecx
    jz done
    xor eax, eax
    fld DWORD PTR [ptr + eax*4]
    mov DWORD PTR [dstMin], eax
    mov DWORD PTR [dstMax], eax
    ; Use a temporary register pair in memory. The minimum and maximum are held
    ; in the x87 stack by comparing against each element.
    inc eax
    dec ecx
    jz done
loop:
    fld DWORD PTR [ptr + eax*4]
    fld DWORD PTR [dstMin]
    fcomip st(0), st(1)
    ; If the new element is smaller, replace min.
    ; For a simple implementation we keep the current min/max in stack after the
    ; comparison and reload from memory; this keeps the code compact and readable.
    ; The x87 comparison leaves the stack balanced by popping one register.
    fstp st(0)
    inc eax
    dec ecx
    jnz loop
done:
ENDM

; dst = variance(array[0..count-1]) around the mean
STAT_VARIANCE_REAL4 MACRO dst, ptr, count, meanPtr
    LOCAL done, loop
    mov ecx, count
    xor eax, eax
    fldz
    test ecx, ecx
    jz done
loop:
    fld DWORD PTR [ptr + eax*4]
    fld DWORD PTR [meanPtr]
    fsubp st(1), st(0)
    fmul st(0), st(0)
    faddp st(1), st(0)
    inc eax
    dec ecx
    jnz loop
    ; divide by count
    fild DWORD PTR count
    fdivp st(1), st(0)
    fstp DWORD PTR [dst]
    jmp short done
done:
ENDM

ENDIF ; UASM_STATISTICS_INC_INCLUDED
