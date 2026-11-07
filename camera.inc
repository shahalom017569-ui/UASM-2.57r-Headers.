; quaternion.inc - single-precision quaternion helpers for UASM 2.57r
; Quaternions are stored as [x, y, z, w]. This file is built on top of
; float.inc and vector.inc and uses SSE. It provides basic composition,
; normalization, and rotation-vector conversion helpers.

INCLUDE float.inc
INCLUDE vector.inc

IFNDEF UASM_QUATERNION_INC_INCLUDED
UASM_QUATERNION_INC_INCLUDED EQU 1

QUAT STRUCT
    x REAL4 ?
    y REAL4 ?
    z REAL4 ?
    w REAL4 ?
QUAT ENDS

; q = [x, y, z, w]
; q * q2 = quat_mul(q, q2)
Q_MUL MACRO dst, q1, q2
    ; temp = q1 * q2, where formula: 
    ; (x1 y2 + y1 x2 + z1 w2 - w1 z2, ... )
    ; we compute directly for each component.
    movss xmm0, DWORD PTR [q1+0]
    movss xmm1, DWORD PTR [q2+0]
    movss xmm2, DWORD PTR [q1+4]
    movss xmm3, DWORD PTR [q2+4]
    ; x = x1*w2 + w1*x2 + y1*z2 - z1*y2
    movss xmm4, DWORD PTR [q1+12]
    movss xmm5, DWORD PTR [q2+12]
    mulss xmm0, xmm5
    mulss xmm4, xmm1
    addss xmm0, xmm4
    movss xmm4, DWORD PTR [q1+4]
    mulss xmm4, DWORD PTR [q2+8]
    addss xmm0, xmm4
    movss xmm4, DWORD PTR [q1+8]
    mulss xmm4, DWORD PTR [q2+4]
    subss xmm0, xmm4
    movss DWORD PTR [dst+0], xmm0

    ; y = y1*w2 + w1*y2 + z1*x2 - x1*z2
    movss xmm0, DWORD PTR [q1+4]
    mulss xmm0, xmm5
    movss xmm1, DWORD PTR [q1+12]
    mulss xmm1, DWORD PTR [q2+4]
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [q1+8]
    mulss xmm1, DWORD PTR [q2+0]
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [q1+0]
    mulss xmm1, DWORD PTR [q2+8]
    subss xmm0, xmm1
    movss DWORD PTR [dst+4], xmm0

    ; z = z1*w2 + w1*z2 + x1*y2 - y1*x2
    movss xmm0, DWORD PTR [q1+8]
    mulss xmm0, xmm5
    movss xmm1, DWORD PTR [q1+12]
    mulss xmm1, DWORD PTR [q2+8]
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [q1+0]
    mulss xmm1, DWORD PTR [q2+4]
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [q1+4]
    mulss xmm1, DWORD PTR [q2+0]
    subss xmm0, xmm1
    movss DWORD PTR [dst+8], xmm0

    ; w = w1*w2 - x1*x2 - y1*y2 - z1*z2
    movss xmm0, DWORD PTR [q1+12]
    mulss xmm0, DWORD PTR [q2+12]
    movss xmm1, DWORD PTR [q1+0]
    mulss xmm1, DWORD PTR [q2+0]
    subss xmm0, xmm1
    movss xmm1, DWORD PTR [q1+4]
    mulss xmm1, DWORD PTR [q2+4]
    subss xmm0, xmm1
    movss xmm1, DWORD PTR [q1+8]
    mulss xmm1, DWORD PTR [q2+8]
    subss xmm0, xmm1
    movss DWORD PTR [dst+12], xmm0
ENDM

; q = normalize(q)
Q_NORMALIZE MACRO dst, src
    LOCAL oneOverLen
    DATASEG
    oneOverLen REAL4 0.0
    CODESEG
    movss xmm0, DWORD PTR [src+0]
    mulss xmm0, xmm0
    movss xmm1, DWORD PTR [src+4]
    mulss xmm1, xmm1
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [src+8]
    mulss xmm1, xmm1
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [src+12]
    mulss xmm1, xmm1
    addss xmm0, xmm1
    sqrtss xmm0, xmm0
    movss oneOverLen, xmm0
    movss xmm1, oneOverLen
    xorpd xmm2, xmm2
    movss xmm2, DWORD PTR [src+0]
    divss xmm2, xmm1
    movss DWORD PTR [dst+0], xmm2
    movss xmm2, DWORD PTR [src+4]
    divss xmm2, xmm1
    movss DWORD PTR [dst+4], xmm2
    movss xmm2, DWORD PTR [src+8]
    divss xmm2, xmm1
    movss DWORD PTR [dst+8], xmm2
    movss xmm2, DWORD PTR [src+12]
    divss xmm2, xmm1
    movss DWORD PTR [dst+12], xmm2
ENDM

; Convert axis-angle vector to quaternion.
; input axis = [ax, ay, az], angle = radian angle in memory scalar.
Q_FROM_AXIS_ANGLE MACRO dst, axis, angle
    ; q = [sin(angle/2)*axis, cos(angle/2)]
    movss xmm0, angle
    mulss xmm0, HALF_F32
    sincos??
ENDM

ENDIF ; UASM_QUATERNION_INC_INCLUDED
