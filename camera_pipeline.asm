; camera_pipeline.asm - compact world/view/projection example for UASM 2.57r
; Demonstrates: model transform, view matrix, perspective projection,
; matrix composition, homogeneous point transform, and perspective divide.
;
; This example uses 32-bit registers and is intended for a 32-bit flat model.
; All matrices are row-major and multiply column vectors:
;     clip = projection * view * model * vertex
; The camera is right-handed, looks down -Z, and uses OpenGL NDC depth [-1,1].
; Output is written to clipPoint and ndcPoint in .data for inspection.

.686
.xmm
.model flat
option casemap:none

INCLUDE transform.inc
INCLUDE camera.inc

.data
; Orthonormal camera basis; eye at (0,0,5), looking toward the origin.
cameraRight    VEC3 <1.0, 0.0, 0.0, 0.0>
cameraUp       VEC3 <0.0, 1.0, 0.0, 0.0>
cameraBackward VEC3 <0.0, 0.0, 1.0, 0.0>
cameraEye      VEC3 <0.0, 0.0, 5.0, 0.0>

zeroF32        REAL4 0.0
projXScale     REAL4 1.0
projYScale     REAL4 1.0
nearPlane      REAL4 0.1
farPlane       REAL4 100.0

modelMatrix    MAT4 <>
viewMatrix     MAT4 <>
projection     MAT4 <>
viewModel      MAT4 <>
modelViewProj  MAT4 <>

; A model-space point with homogeneous w=1.
vertex         VEC4 <1.0, 1.0, 0.0, 1.0>
clipPoint      VEC4 <>
ndcPoint       VEC3 <>

.code
start PROC
    ; Model transform: identity translation (replace offsets to move the model).
    lea esi, modelMatrix
    MAT4_TRANSLATION esi, zeroF32, zeroF32, zeroF32

    ; Construct the view matrix from the camera's orthonormal basis.
    lea esi, viewMatrix
    lea edi, cameraRight
    lea ebx, cameraUp
    lea ecx, cameraBackward
    lea edx, cameraEye
    CAMERA_VIEW_BASIS esi, edi, ebx, ecx, edx

    ; Build right-handed perspective projection (near/far must be positive).
    lea esi, projection
    CAMERA_PERSPECTIVE esi, projXScale, projYScale, nearPlane, farPlane

    ; Compose viewModel = view * model, then MVP = projection * viewModel.
    lea esi, viewModel
    lea edi, viewMatrix
    lea ebx, modelMatrix
    MAT4_MUL esi, edi, ebx

    lea esi, modelViewProj
    lea edi, projection
    lea ebx, viewModel
    MAT4_MUL esi, edi, ebx

    ; Transform the vertex to clip coordinates: (x, y, z, w).
    lea esi, clipPoint
    lea edi, modelViewProj
    lea ebx, vertex
    MAT4_TRANSFORM4 esi, edi, ebx

    ; Perspective divide converts clip coordinates to normalized device coords.
    ; The chosen camera/vertex keeps w nonzero and the point in front of camera.
    movss xmm0, DWORD PTR [clipPoint+0]
    divss xmm0, DWORD PTR [clipPoint+12]
    movss DWORD PTR [ndcPoint+0], xmm0
    movss xmm0, DWORD PTR [clipPoint+4]
    divss xmm0, DWORD PTR [clipPoint+12]
    movss DWORD PTR [ndcPoint+4], xmm0
    movss xmm0, DWORD PTR [clipPoint+8]
    divss xmm0, DWORD PTR [clipPoint+12]
    movss DWORD PTR [ndcPoint+8], xmm0

    ; For this example, clipPoint.w is 5.0 and NDC x/y are approximately 0.2.
    xor eax, eax
    ret
start ENDP

END start
