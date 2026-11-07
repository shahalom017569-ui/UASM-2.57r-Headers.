; camera.inc - view and projection matrices for a right-handed 3D camera
; Requires MAT4 from matrix.inc. Matrix layout is row-major and vectors are
; columns: clip = projection * view * position. Camera looks down -Z.
;
; CAMERA_VIEW_BASIS takes orthonormal right/up/backward vectors and eye position
; (each VEC3 base register); rows become right, up, backward, with translation
; -dot(axis, eye). For a conventional look-at camera, backward = normalize(eye-target),
; right = normalize(cross(upHint, backward)), up = cross(backward, right).
;
; Projection helpers take precomputed scalar parameters to avoid hidden trig:
; CAMERA_PERSPECTIVE takes xScale=cot(fovY/2)/aspect and yScale=cot(fovY/2),
; plus positive near/far distances. It emits an OpenGL-style NDC depth range [-1,1].
; CAMERA_ORTHO takes left/right/bottom/top and positive near/far distances,
; also using OpenGL-style NDC depth [-1,1]. Inputs must be valid/non-degenerate.

INCLUDE matrix.inc
INCLUDE vector.inc

IFNDEF UASM_CAMERA_INC_INCLUDED
UASM_CAMERA_INC_INCLUDED EQU 1

; Build a right-handed view matrix from already normalized orthogonal basis axes.
; dst, right, up, backward, eye are base registers. Clobbers XMM0-XMM2.
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
    ; Translation components: -dot(axis, eye).
    movss xmm0, DWORD PTR [right+0]
    mulss xmm0, DWORD PTR [eye+0]
    movss xmm1, DWORD PTR [right+4]
    mulss xmm1, DWORD PTR [eye+4]
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [right+8]
    mulss xmm1, DWORD PTR [eye+8]
    addss xmm0, xmm1
    xor eax, eax
    movd xmm1, eax
    subss xmm1, xmm0
    movss DWORD PTR [dst+12], xmm1
    movss xmm0, DWORD PTR [up+0]
    mulss xmm0, DWORD PTR [eye+0]
    movss xmm1, DWORD PTR [up+4]
    mulss xmm1, DWORD PTR [eye+4]
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [up+8]
    mulss xmm1, DWORD PTR [eye+8]
    addss xmm0, xmm1
    xor eax, eax
    movd xmm1, eax
    subss xmm1, xmm0
    movss DWORD PTR [dst+28], xmm1
    movss xmm0, DWORD PTR [backward+0]
    mulss xmm0, DWORD PTR [eye+0]
    movss xmm1, DWORD PTR [backward+4]
    mulss xmm1, DWORD PTR [eye+4]
    addss xmm0, xmm1
    movss xmm1, DWORD PTR [backward+8]
    mulss xmm1, DWORD PTR [eye+8]
    addss xmm0, xmm1
    xor eax, eax
    movd xmm1, eax
    subss xmm1, xmm0
    movss DWORD PTR [dst+44], xmm1
ENDM

; Perspective projection: xScale and yScale supplied by caller, see header.
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
    ; m32 = -1
    mov DWORD PTR [dst+56], 0BF800000h
ENDM

; Orthographic projection, right-handed with OpenGL-style NDC depth [-1,1].
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
