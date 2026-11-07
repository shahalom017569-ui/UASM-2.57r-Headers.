; camera.inc - right-handed camera view/projection helpers for UASM 2.57r
; Depends on MAT4 from matrix.inc and VEC3 from vector.inc.
;
; Matrices are row-major and multiply column vectors. Camera space looks down
; -Z. Projection follows OpenGL NDC depth [-1,+1]. Parameters are REAL4 memory
; operands; dst/axis/eye arguments are base registers.
;
; CAMERA_VIEW_BASIS expects orthonormal right/up/backward axes and eye position.
; It builds the view matrix rows from those axes and translation -dot(axis,eye).
; CAMERA_PERSPECTIVE expects xScale=cot(fovY/2)/aspect and
; yScale=cot(fovY/2), plus positive near/far distances (far > near > 0).
; CAMERA_ORTHO expects valid left/right, bottom/top, and positive near/far.
; These routines use SSE2 and clobber XMM0-XMM2 and EAX.

INCLUDE vector.inc
INCLUDE matrix.inc

IFNDEF UASM_CAMERA_INC_INCLUDED
UASM_CAMERA_INC_INCLUDED EQU 1

CAMERA_VIEW_BASIS MACRO dst, right, up, backward, eye
    MAT4_IDENTITY dst
    movss xmm0, DWORD PTR [right+0]
    movss DWORD PTR [dst+0], xmm0
    movss xmm0, DWORD PTR [right+4]
    movss DWORD PTR [dst+4], xmm0
    movss xmm0, DWORD PTR [right+8]
    movss DWORD PTR [dst+8], xmm0

    movss xmm0, DWORD PTR [up+0]
    movss DWORD PTR [dst+16], xmm0
    movss xmm0, DWORD PTR [up+4]
    movss DWORD PTR [dst+20], xmm0
    movss xmm0, DWORD PTR [up+8]
    movss DWORD PTR [dst+24], xmm0

    movss xmm0, DWORD PTR [backward+0]
    movss DWORD PTR [dst+32], xmm0
    movss xmm0, DWORD PTR [backward+4]
    movss DWORD PTR [dst+36], xmm0
    movss xmm0, DWORD PTR [backward+8]
    movss DWORD PTR [dst+40], xmm0

    ; m03 = -dot(right, eye)
    movss xmm0, DWORD PTR [right+0]
    mulss xmm0, DWORD PTR [eye+0]
    movss xmm1, DWORD PTR [right+4]
    mulss xmm1, DWORD PTR [eye+4]
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [right+8]
    mulss xmm1, DWORD PTR [eye+8]
    addss xmm0, xmm1
    movd eax, xmm0
    xor eax, 080000000h
    movd xmm0, eax
    movss DWORD PTR [dst+12], xmm0

    ; m13 = -dot(up, eye)
    movss xmm0, DWORD PTR [up+0]
    mulss xmm0, DWORD PTR [eye+0]
    movss xmm1, DWORD PTR [up+4]
    mulss xmm1, DWORD PTR [eye+4]
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [up+8]
    mulss xmm1, DWORD PTR [eye+8]
    addss xmm0, xmm1
    movd eax, xmm0
    xor eax, 080000000h
    movd xmm0, eax
    movss DWORD PTR [dst+28], xmm0

    ; m23 = -dot(backward, eye)
    movss xmm0, DWORD PTR [backward+0]
    mulss xmm0, DWORD PTR [eye+0]
    movss xmm1, DWORD PTR [backward+4]
    mulss xmm1, DWORD PTR [eye+4]
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [backward+8]
    mulss xmm1, DWORD PTR [eye+8]
    addss xmm0, xmm1
    movd eax, xmm0
    xor eax, 080000000h
    movd xmm0, eax
    movss DWORD PTR [dst+44], xmm0
ENDM

; Right-handed perspective matrix with OpenGL-style NDC z in [-1,+1].
CAMERA_PERSPECTIVE MACRO dst, xScale, yScale, nearZ, farZ
    MAT4_ZERO dst
    movss xmm0, xScale
    movss DWORD PTR [dst+0], xmm0
    movss xmm0, yScale
    movss DWORD PTR [dst+20], xmm0

    ; m22 = -(far + near) / (far - near)
    movss xmm0, farZ
    addss xmm0, nearZ
    movss xmm1, farZ
    subss xmm1, nearZ
    divss xmm0, xmm1
    movd eax, xmm0
    xor eax, 080000000h
    movd xmm0, eax
    movss DWORD PTR [dst+40], xmm0

    ; m23 = -(2 * far * near) / (far - near)
    movss xmm0, farZ
    mulss xmm0, nearZ
    addss xmm0, xmm0
    divss xmm0, xmm1
    movd eax, xmm0
    xor eax, 080000000h
    movd xmm0, eax
    movss DWORD PTR [dst+44], xmm0
    mov DWORD PTR [dst+56], 0BF800000h ; m32 = -1
ENDM

; Right-handed orthographic projection with OpenGL-style NDC z in [-1,+1].
CAMERA_ORTHO MACRO dst, left, right, bottom, top, nearZ, farZ
    MAT4_IDENTITY dst

    ; m00 = 2/(right-left), m03 = -(right+left)/(right-left)
    movss xmm0, right
    subss xmm0, left
    movss xmm1, TWO_F32
    divss xmm1, xmm0
    movss DWORD PTR [dst+0], xmm1
    movss xmm1, right
    addss xmm1, left
    divss xmm1, xmm0
    movd eax, xmm1
    xor eax, 080000000h
    movd xmm1, eax
    movss DWORD PTR [dst+12], xmm1

    ; m11 = 2/(top-bottom), m13 = -(top+bottom)/(top-bottom)
    movss xmm0, top
    subss xmm0, bottom
    movss xmm1, TWO_F32
    divss xmm1, xmm0
    movss DWORD PTR [dst+20], xmm1
    movss xmm1, top
    addss xmm1, bottom
    divss xmm1, xmm0
    movd eax, xmm1
    xor eax, 080000000h
    movd xmm1, eax
    movss DWORD PTR [dst+28], xmm1

    ; m22 = -2/(far-near), m23 = -(far+near)/(far-near)
    movss xmm0, farZ
    subss xmm0, nearZ
    movss xmm1, TWO_F32
    divss xmm1, xmm0
    movd eax, xmm1
    xor eax, 080000000h
    movd xmm1, eax
    movss DWORD PTR [dst+40], xmm1
    movss xmm1, farZ
    addss xmm1, nearZ
    divss xmm1, xmm0
    movd eax, xmm1
    xor eax, 080000000h
    movd xmm1, eax
    movss DWORD PTR [dst+44], xmm1
ENDM

TWO_F32 REAL4 2.0

ENDIF ; UASM_CAMERA_INC_INCLUDED
