; transform_pipeline.asm - compact 2D/3D transform example for UASM
; Assemble as x86 or x64 with SSE enabled for x86.

INCLUDE float.inc
INCLUDE transform.inc

.data
angleSin REAL4 0.0
angleCos REAL4 1.0
scaleX   REAL4 2.0
scaleY   REAL4 2.0
scaleZ   REAL4 2.0
moveX    REAL4 10.0
moveY    REAL4 20.0
moveZ    REAL4 30.0
scalarA  REAL4 3.0
scalarB  REAL4 4.0
scalarOut REAL4 ?

point2   VEC2 <1.0, 2.0>
point2Out VEC2 <>
translate2 MAT3 <>
rotate2    MAT3 <>
scale2     MAT3 <>
combined2  MAT3 <>

point3    VEC3 <1.0, 2.0, 3.0, 0.0>
point3Out VEC3 <>
translate3 MAT4 <>
rotateZ3   MAT4 <>
scale3     MAT4 <>
combined3  MAT4 <>
point4    VEC4 <1.0, 2.0, 3.0, 1.0>
clip4     VEC4 <>

.code
start PROC
    F32_ADD scalarOut, scalarA, scalarB

    ; Build a 2D transform: translate * rotate * scale.
    lea esi, translate2
    MAT3_TRANSLATION esi, moveX, moveY
    lea esi, rotate2
    MAT3_ROTATION esi, angleSin, angleCos
    lea esi, scale2
    MAT3_SCALE esi, scaleX, scaleY
    lea esi, combined2
    lea edi, rotate2
    lea ebx, scale2
    MAT3_MUL esi, edi, ebx
    lea edi, combined2
    lea ebx, translate2
    MAT3_MUL esi, ebx, edi
    lea esi, point2Out
    lea edi, combined2
    lea ebx, point2
    MAT3_TRANSFORM_POINT2 esi, edi, ebx

    ; Build a 3D affine transform and transform a point with implicit w=1.
    lea esi, translate3
    MAT4_TRANSLATION esi, moveX, moveY, moveZ
    lea esi, rotateZ3
    MAT4_ROT_Z esi, angleSin, angleCos
    lea esi, scale3
    MAT4_SCALE esi, scaleX, scaleY, scaleZ
    lea esi, combined3
    lea edi, rotateZ3
    lea ebx, scale3
    MAT4_MUL esi, edi, ebx
    lea edi, combined3
    lea ebx, translate3
    MAT4_MUL esi, ebx, edi
    lea esi, point3Out
    lea edi, combined3
    lea ebx, point3
    MAT4_TRANSFORM_POINT3 esi, edi, ebx

    ; Homogeneous transform (useful for view/projection pipelines).
    lea esi, clip4
    lea edi, combined3
    lea ebx, point4
    MAT4_TRANSFORM4 esi, edi, ebx

    ret
start ENDP
END start
