; math_vectors.asm - demonstration of the math/vector/matrix includes
; Assemble as 32-bit for this example. SSE is required by vector.inc.

INCLUDE math.inc
INCLUDE vector.inc
INCLUDE matrix.inc

.data
scalarA REAL4 3.0
scalarB REAL4 4.0
scalarResult REAL4 ?
vecA VEC3 <1.0, 2.0, 3.0, 0.0>
vecB VEC3 <4.0, 5.0, 6.0, 0.0>
vecResult VEC3 <0.0, 0.0, 0.0, 0.0>
matA MAT4 <>
matB MAT4 <>
matResult MAT4 <>

.code
start PROC
    MATH_F32_ADD scalarResult, scalarA, scalarB

    lea esi, vecA
    lea edi, vecB
    lea ebx, vecResult
    VEC3_ADD ebx, esi, edi
    VEC3_DOT esi, edi
    ; XMM0 contains dot(vecA, vecB), which is 32.0.

    lea esi, matA
    MAT4_IDENTITY esi
    lea edi, matB
    MAT4_IDENTITY edi
    lea ebx, matResult
    MAT4_MUL ebx, esi, edi

    ret
start ENDP
END start
