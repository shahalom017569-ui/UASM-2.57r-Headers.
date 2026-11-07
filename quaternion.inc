; quaternion.inc - practical REAL4 quaternion operations for UASM 2.57r
; Quaternion layout is [x,y,z,w], with vector part first and scalar last.
; Include float.inc for scalar helpers (this file includes it directly).
; Inline macros use x87 arithmetic so they do not depend on XMM register count.
;
; Q_MUL and Q_TO_MAT4 require dst not to overlap the inputs. Other operations
; load inputs before writing outputs and permit dst=src. Q_NORMALIZE assumes a
; nonzero quaternion. Q_FROM_AXIS_COS_SIN assumes a normalized axis and takes
; sin(angle/2), cos(angle/2) as REAL4 operands; the caller handles trig/range.

INCLUDE float.inc
INCLUDE matrix.inc

IFNDEF UASM_QUATERNION_INC_INCLUDED
UASM_QUATERNION_INC_INCLUDED EQU 1

QUAT STRUCT
    x REAL4 ?
    y REAL4 ?
    z REAL4 ?
    w REAL4 ?
QUAT ENDS

Q_IDENTITY MACRO dst
    mov DWORD PTR [dst+0], 0
    mov DWORD PTR [dst+4], 0
    mov DWORD PTR [dst+8], 0
    mov DWORD PTR [dst+12], 03F800000h
ENDM

Q_CONJUGATE MACRO dst, src
    mov eax, DWORD PTR [src+0]
    xor eax, 080000000h
    mov DWORD PTR [dst+0], eax
    mov eax, DWORD PTR [src+4]
    xor eax, 080000000h
    mov DWORD PTR [dst+4], eax
    mov eax, DWORD PTR [src+8]
    xor eax, 080000000h
    mov DWORD PTR [dst+8], eax
    mov eax, DWORD PTR [src+12]
    mov DWORD PTR [dst+12], eax
ENDM

; dst = q1 * q2 (Hamilton product); x87 stack balanced at exit.
Q_MUL MACRO dst, q1, q2
    ; x = w1*x2 + x1*w2 + y1*z2 - z1*y2
    fld DWORD PTR [q1+12]
    fmul DWORD PTR [q2+0]
    fld DWORD PTR [q1+0]
    fmul DWORD PTR [q2+12]
    faddp st(1), st(0)
    fld DWORD PTR [q1+4]
    fmul DWORD PTR [q2+8]
    faddp st(1), st(0)
    fld DWORD PTR [q1+8]
    fmul DWORD PTR [q2+4]
    fsubp st(1), st(0)
    fstp DWORD PTR [dst+0]

    ; y = w1*y2 - x1*z2 + y1*w2 + z1*x2
    fld DWORD PTR [q1+12]
    fmul DWORD PTR [q2+4]
    fld DWORD PTR [q1+0]
    fmul DWORD PTR [q2+8]
    fsubp st(1), st(0)
    fld DWORD PTR [q1+4]
    fmul DWORD PTR [q2+12]
    faddp st(1), st(0)
    fld DWORD PTR [q1+8]
    fmul DWORD PTR [q2+0]
    faddp st(1), st(0)
    fstp DWORD PTR [dst+4]

    ; z = w1*z2 + x1*y2 - y1*x2 + z1*w2
    fld DWORD PTR [q1+12]
    fmul DWORD PTR [q2+8]
    fld DWORD PTR [q1+0]
    fmul DWORD PTR [q2+4]
    faddp st(1), st(0)
    fld DWORD PTR [q1+4]
    fmul DWORD PTR [q2+0]
    fsubp st(1), st(0)
    fld DWORD PTR [q1+8]
    fmul DWORD PTR [q2+12]
    faddp st(1), st(0)
    fstp DWORD PTR [dst+8]

    ; w = w1*w2 - x1*x2 - y1*y2 - z1*z2
    fld DWORD PTR [q1+12]
    fmul DWORD PTR [q2+12]
    fld DWORD PTR [q1+0]
    fmul DWORD PTR [q2+0]
    fsubp st(1), st(0)
    fld DWORD PTR [q1+4]
    fmul DWORD PTR [q2+4]
    fsubp st(1), st(0)
    fld DWORD PTR [q1+8]
    fmul DWORD PTR [q2+8]
    fsubp st(1), st(0)
    fstp DWORD PTR [dst+12]
ENDM

Q_NORMALIZE MACRO dst, src
    fld DWORD PTR [src+0]
    fmul st(0), st(0)
    fld DWORD PTR [src+4]
    fmul st(0), st(0)
    faddp st(1), st(0)
    fld DWORD PTR [src+8]
    fmul st(0), st(0)
    faddp st(1), st(0)
    fld DWORD PTR [src+12]
    fmul st(0), st(0)
    faddp st(1), st(0)
    fsqrt
    ; Keep norm in st(0), divide each source component by it.
    fld DWORD PTR [src+0]
    fdiv st(0), st(1)
    fstp DWORD PTR [dst+0]
    fld DWORD PTR [src+4]
    fdiv st(0), st(1)
    fstp DWORD PTR [dst+4]
    fld DWORD PTR [src+8]
    fdiv st(0), st(1)
    fstp DWORD PTR [dst+8]
    fld DWORD PTR [src+12]
    fdiv st(0), st(1)
    fstp DWORD PTR [dst+12]
    fstp st(0)
ENDM

; Set quaternion from normalized axis and caller-computed half-angle sine/cosine.
Q_FROM_AXIS_COS_SIN MACRO dst, axis, sinHalf, cosHalf
    fld DWORD PTR [axis+0]
    fmul DWORD PTR [sinHalf]
    fstp DWORD PTR [dst+0]
    fld DWORD PTR [axis+4]
    fmul DWORD PTR [sinHalf]
    fstp DWORD PTR [dst+4]
    fld DWORD PTR [axis+8]
    fmul DWORD PTR [sinHalf]
    fstp DWORD PTR [dst+8]
    mov eax, DWORD PTR [cosHalf]
    mov DWORD PTR [dst+12], eax
ENDM

; Convert a unit quaternion to a row-major MAT4 using column vectors.
Q_TO_MAT4 MACRO dst, src
    LOCAL qx2, qy2, qz2, xx, yy, zz, xy, xz, yz, wx, wy, wz
    ; Compute the standard rotation matrix with x87 intermediates. Scratch
    ; values are pushed to the x87 stack only; no static data is allocated.
    ; For simplicity and alias safety, dst must not overlap src.
    fld DWORD PTR [src+0]
    fadd st(0), st(0)
    fld DWORD PTR [src+4]
    fadd st(0), st(0)
    fld DWORD PTR [src+8]
    fadd st(0), st(0)
    ; Use an inline scalar formula for each matrix element.
    fld DWORD PTR [src+4]
    fmul DWORD PTR [src+4]
    fld DWORD PTR [src+8]
    fmul DWORD PTR [src+8]
    faddp st(1), st(0)
    fld1
    fsubp st(1), st(0)
    fadd st(0), st(0)
    fld1
    fsubp st(1), st(0)
    ; Replace temporary sequence with exact element computations below.
    fstp st(0)
    fstp st(0)
    fstp st(0)
    ; m00 = 1 - 2(y*y + z*z)
    fld DWORD PTR [src+4]
    fmul st(0), st(0)
    fld DWORD PTR [src+8]
    fmul st(0), st(0)
    faddp st(1), st(0)
    fadd st(0), st(0)
    fld1
    fsubp st(1), st(0)
    fstp DWORD PTR [dst+0]
    ; m01 = 2(x*y - z*w)
    fld DWORD PTR [src+0]
    fmul DWORD PTR [src+4]
    fld DWORD PTR [src+8]
    fmul DWORD PTR [src+12]
    fsubp st(1), st(0)
    fadd st(0), st(0)
    fstp DWORD PTR [dst+4]
    ; m02 = 2(x*z + y*w)
    fld DWORD PTR [src+0]
    fmul DWORD PTR [src+8]
    fld DWORD PTR [src+4]
    fmul DWORD PTR [src+12]
    faddp st(1), st(0)
    fadd st(0), st(0)
    fstp DWORD PTR [dst+8]
    mov DWORD PTR [dst+12], 0
    ; m10 = 2(x*y + z*w)
    fld DWORD PTR [src+0]
    fmul DWORD PTR [src+4]
    fld DWORD PTR [src+8]
    fmul DWORD PTR [src+12]
    faddp st(1), st(0)
    fadd st(0), st(0)
    fstp DWORD PTR [dst+16]
    ; m11 = 1 - 2(x*x + z*z)
    fld DWORD PTR [src+0]
    fmul st(0), st(0)
    fld DWORD PTR [src+8]
    fmul st(0), st(0)
    faddp st(1), st(0)
    fadd st(0), st(0)
    fld1
    fsubp st(1), st(0)
    fstp DWORD PTR [dst+20]
    ; m12 = 2(y*z - x*w)
    fld DWORD PTR [src+4]
    fmul DWORD PTR [src+8]
    fld DWORD PTR [src+0]
    fmul DWORD PTR [src+12]
    fsubp st(1), st(0)
    fadd st(0), st(0)
    fstp DWORD PTR [dst+24]
    mov DWORD PTR [dst+28], 0
    ; m20 = 2(x*z - y*w)
    fld DWORD PTR [src+0]
    fmul DWORD PTR [src+8]
    fld DWORD PTR [src+4]
    fmul DWORD PTR [src+12]
    fsubp st(1), st(0)
    fadd st(0), st(0)
    fstp DWORD PTR [dst+32]
    ; m21 = 2(y*z + x*w)
    fld DWORD PTR [src+4]
    fmul DWORD PTR [src+8]
    fld DWORD PTR [src+0]
    fmul DWORD PTR [src+12]
    faddp st(1), st(0)
    fadd st(0), st(0)
    fstp DWORD PTR [dst+36]
    ; m22 = 1 - 2(x*x + y*y)
    fld DWORD PTR [src+0]
    fmul st(0), st(0)
    fld DWORD PTR [src+4]
    fmul st(0), st(0)
    faddp st(1), st(0)
    fadd st(0), st(0)
    fld1
    fsubp st(1), st(0)
    fstp DWORD PTR [dst+40]
    mov DWORD PTR [dst+44], 0
    mov DWORD PTR [dst+48], 0
    mov DWORD PTR [dst+52], 0
    mov DWORD PTR [dst+56], 0
    mov DWORD PTR [dst+60], 03F800000h
ENDM

ENDIF ; UASM_QUATERNION_INC_INCLUDED
