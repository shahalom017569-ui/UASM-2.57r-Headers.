; transform.inc - 2D affine and 3D homogeneous transform helpers
; UASM 2.57r, MASM-style syntax. Includes vector.inc and matrix.inc.
;
; Matrices are row-major and use column vectors: result = M * vector.
; Translation is in the last column. Matrix/vector parameters are base
; registers. Constructors take REAL4 memory operands. Trig rotation constructors
; take caller-computed sine and cosine REAL4 values (radians); no hidden runtime
; trig approximation is used. SSE is required for vector transforms.
;
; MAT3 is a 3x3 2D homogeneous matrix (36 bytes).
; MAT4 is the 4x4 matrix from matrix.inc (64 bytes).
; MAT3_MUL/MAT4_MUL write dst and require dst not to overlap either input.
; Point/vector transforms may write in place because source lanes are captured
; before output stores. MAT4_PROJECT_POINT4 divides xyz by w; caller must ensure
; w is nonzero and handle clipping/near-plane policy.

INCLUDE vector.inc
INCLUDE matrix.inc

IFNDEF UASM_TRANSFORM_INC_INCLUDED
UASM_TRANSFORM_INC_INCLUDED EQU 1

MAT3 STRUCT
    m00 REAL4 ?
    m01 REAL4 ?
    m02 REAL4 ?
    m10 REAL4 ?
    m11 REAL4 ?
    m12 REAL4 ?
    m20 REAL4 ?
    m21 REAL4 ?
    m22 REAL4 ?
MAT3 ENDS

VEC4 STRUCT
    x REAL4 ?
    y REAL4 ?
    z REAL4 ?
    w REAL4 ?
VEC4 ENDS

MAT3_IDENTITY MACRO dst
    mov DWORD PTR [dst+0], 03F800000h
    mov DWORD PTR [dst+4], 0
    mov DWORD PTR [dst+8], 0
    mov DWORD PTR [dst+12], 0
    mov DWORD PTR [dst+16], 03F800000h
    mov DWORD PTR [dst+20], 0
    mov DWORD PTR [dst+24], 0
    mov DWORD PTR [dst+28], 0
    mov DWORD PTR [dst+32], 03F800000h
ENDM

; 2D affine constructors, for row-major/column-vector convention.
MAT3_TRANSLATION MACRO dst, tx, ty
    MAT3_IDENTITY dst
    movss xmm0, tx
    movss DWORD PTR [dst+8], xmm0
    movss xmm0, ty
    movss DWORD PTR [dst+20], xmm0
ENDM

MAT3_SCALE MACRO dst, sx, sy
    MAT3_IDENTITY dst
    movss xmm0, sx
    movss DWORD PTR [dst+0], xmm0
    movss xmm0, sy
    movss DWORD PTR [dst+16], xmm0
ENDM

; Rotation: rows [cos,-sin,0], [sin,cos,0], [0,0,1].
MAT3_ROTATION MACRO dst, sinValue, cosValue
    MAT3_IDENTITY dst
    movss xmm0, cosValue
    movss DWORD PTR [dst+0], xmm0
    movss DWORD PTR [dst+16], xmm0
    movss xmm0, sinValue
    movss DWORD PTR [dst+12], xmm0
    movd eax, xmm0
    xor eax, 080000000h
    movd xmm0, eax
    movss DWORD PTR [dst+4], xmm0
ENDM

MAT4_TRANSLATION MACRO dst, tx, ty, tz
    MAT4_IDENTITY dst
    movss xmm0, tx
    movss DWORD PTR [dst+12], xmm0
    movss xmm0, ty
    movss DWORD PTR [dst+28], xmm0
    movss xmm0, tz
    movss DWORD PTR [dst+44], xmm0
ENDM

MAT4_SCALE MACRO dst, sx, sy, sz
    MAT4_ZERO dst
    movss xmm0, sx
    movss DWORD PTR [dst+0], xmm0
    movss xmm0, sy
    movss DWORD PTR [dst+20], xmm0
    movss xmm0, sz
    movss DWORD PTR [dst+40], xmm0
    mov DWORD PTR [dst+60], 03F800000h
ENDM

; Rotation constructors receive sin(angle), cos(angle) as REAL4 operands.
MAT4_ROT_X MACRO dst, sinValue, cosValue
    MAT4_IDENTITY dst
    movss xmm0, cosValue
    movss DWORD PTR [dst+20], xmm0
    movss DWORD PTR [dst+40], xmm0
    movss xmm0, sinValue
    movss DWORD PTR [dst+36], xmm0
    movd eax, xmm0
    xor eax, 080000000h
    movd xmm0, eax
    movss DWORD PTR [dst+24], xmm0
ENDM

MAT4_ROT_Y MACRO dst, sinValue, cosValue
    MAT4_IDENTITY dst
    movss xmm0, cosValue
    movss DWORD PTR [dst+0], xmm0
    movss DWORD PTR [dst+40], xmm0
    movss xmm0, sinValue
    movss DWORD PTR [dst+8], xmm0
    movd eax, xmm0
    xor eax, 080000000h
    movd xmm0, eax
    movss DWORD PTR [dst+32], xmm0
ENDM

MAT4_ROT_Z MACRO dst, sinValue, cosValue
    MAT4_IDENTITY dst
    movss xmm0, cosValue
    movss DWORD PTR [dst+0], xmm0
    movss DWORD PTR [dst+20], xmm0
    movss xmm0, sinValue
    movss DWORD PTR [dst+16], xmm0
    movd eax, xmm0
    xor eax, 080000000h
    movd xmm0, eax
    movss DWORD PTR [dst+4], xmm0
ENDM

; Matrix multiplication cell helpers (x87 stack is balanced after each cell).
MAT3_MUL_CELL MACRO dst, lhs, rhs, outOff, rowOff, colOff
    fld DWORD PTR [lhs+rowOff+0]
    fmul DWORD PTR [rhs+colOff+0]
    fld DWORD PTR [lhs+rowOff+4]
    fmul DWORD PTR [rhs+colOff+12]
    faddp st(1), st(0)
    fld DWORD PTR [lhs+rowOff+8]
    fmul DWORD PTR [rhs+colOff+24]
    faddp st(1), st(0)
    fstp DWORD PTR [dst+outOff]
ENDM

MAT3_MUL MACRO dst, lhs, rhs
    MAT3_MUL_CELL dst, lhs, rhs, 0, 0, 0
    MAT3_MUL_CELL dst, lhs, rhs, 4, 0, 4
    MAT3_MUL_CELL dst, lhs, rhs, 8, 0, 8
    MAT3_MUL_CELL dst, lhs, rhs, 12, 12, 0
    MAT3_MUL_CELL dst, lhs, rhs, 16, 12, 4
    MAT3_MUL_CELL dst, lhs, rhs, 20, 12, 8
    MAT3_MUL_CELL dst, lhs, rhs, 24, 24, 0
    MAT3_MUL_CELL dst, lhs, rhs, 28, 24, 4
    MAT3_MUL_CELL dst, lhs, rhs, 32, 24, 8
ENDM

MAT4_MUL_CELL MACRO dst, lhs, rhs, outOff, rowOff, colOff
    fld DWORD PTR [lhs+rowOff+0]
    fmul DWORD PTR [rhs+colOff+0]
    fld DWORD PTR [lhs+rowOff+4]
    fmul DWORD PTR [rhs+colOff+16]
    faddp st(1), st(0)
    fld DWORD PTR [lhs+rowOff+8]
    fmul DWORD PTR [rhs+colOff+32]
    faddp st(1), st(0)
    fld DWORD PTR [lhs+rowOff+12]
    fmul DWORD PTR [rhs+colOff+48]
    faddp st(1), st(0)
    fstp DWORD PTR [dst+outOff]
ENDM

MAT4_MUL MACRO dst, lhs, rhs
    MAT4_MUL_CELL dst, lhs, rhs, 0, 0, 0
    MAT4_MUL_CELL dst, lhs, rhs, 4, 0, 4
    MAT4_MUL_CELL dst, lhs, rhs, 8, 0, 8
    MAT4_MUL_CELL dst, lhs, rhs, 12, 0, 12
    MAT4_MUL_CELL dst, lhs, rhs, 16, 16, 0
    MAT4_MUL_CELL dst, lhs, rhs, 20, 16, 4
    MAT4_MUL_CELL dst, lhs, rhs, 24, 16, 8
    MAT4_MUL_CELL dst, lhs, rhs, 28, 16, 12
    MAT4_MUL_CELL dst, lhs, rhs, 32, 32, 0
    MAT4_MUL_CELL dst, lhs, rhs, 36, 32, 4
    MAT4_MUL_CELL dst, lhs, rhs, 40, 32, 8
    MAT4_MUL_CELL dst, lhs, rhs, 44, 32, 12
    MAT4_MUL_CELL dst, lhs, rhs, 48, 48, 0
    MAT4_MUL_CELL dst, lhs, rhs, 52, 48, 4
    MAT4_MUL_CELL dst, lhs, rhs, 56, 48, 8
    MAT4_MUL_CELL dst, lhs, rhs, 60, 48, 12
ENDM

; Transform a 2D point using a homogeneous MAT3 (input/output VEC2).
; Clobbers XMM0-XMM3. dst may equal src.
MAT3_TRANSFORM_POINT2 MACRO dst, mat, src
    movss xmm4, DWORD PTR [src+0]
    movss xmm5, DWORD PTR [src+4]
    movss xmm0, DWORD PTR [mat+0]
    mulss xmm0, xmm4
    movss xmm1, DWORD PTR [mat+4]
    mulss xmm1, xmm5
    addss xmm0, xmm1
    addss xmm0, DWORD PTR [mat+8]
    movss DWORD PTR [dst+0], xmm0
    movss xmm0, DWORD PTR [mat+12]
    mulss xmm0, xmm4
    movss xmm1, DWORD PTR [mat+16]
    mulss xmm1, xmm5
    addss xmm0, xmm1
    addss xmm0, DWORD PTR [mat+20]
    movss DWORD PTR [dst+4], xmm0
ENDM

; Transform homogeneous VEC4 by MAT4. dst may equal src.
; Clobbers XMM0-XMM7. Row-major matrix, column-vector convention.
MAT4_TRANSFORM4 MACRO dst, mat, src
    movss xmm4, DWORD PTR [src+0]
    movss xmm5, DWORD PTR [src+4]
    movss xmm6, DWORD PTR [src+8]
    movss xmm7, DWORD PTR [src+12]
    TRANSFORM4_ROW dst, mat, 0, 0
    TRANSFORM4_ROW dst, mat, 4, 16
    TRANSFORM4_ROW dst, mat, 8, 32
    TRANSFORM4_ROW dst, mat, 12, 48
ENDM

TRANSFORM4_ROW MACRO dst, mat, outOff, rowOff
    movaps xmm0, xmm4
    mulss xmm0, DWORD PTR [mat+rowOff+0]
    movaps xmm1, xmm5
    mulss xmm1, DWORD PTR [mat+rowOff+4]
    addss xmm0, xmm1
    movaps xmm1, xmm6
    mulss xmm1, DWORD PTR [mat+rowOff+8]
    addss xmm0, xmm1
    movaps xmm1, xmm7
    mulss xmm1, DWORD PTR [mat+rowOff+12]
    addss xmm0, xmm1
    movss DWORD PTR [dst+outOff], xmm0
ENDM

; Transform a VEC3 point by an affine MAT4 (implicit homogeneous w=1).
; The fourth row is not used; dst may equal src. Clobbers XMM0-XMM6 and EAX.
MAT4_TRANSFORM_POINT3 MACRO dst, mat, src
    movss xmm4, DWORD PTR [src+0]
    movss xmm5, DWORD PTR [src+4]
    movss xmm6, DWORD PTR [src+8]
    movss xmm0, xmm4
    mulss xmm0, DWORD PTR [mat+0]
    movss xmm1, xmm5
    mulss xmm1, DWORD PTR [mat+4]
    addss xmm0, xmm1
    movss xmm1, xmm6
    mulss xmm1, DWORD PTR [mat+8]
    addss xmm0, xmm1
    addss xmm0, DWORD PTR [mat+12]
    movss DWORD PTR [dst+0], xmm0
    movss xmm0, xmm4
    mulss xmm0, DWORD PTR [mat+16]
    movss xmm1, xmm5
    mulss xmm1, DWORD PTR [mat+20]
    addss xmm0, xmm1
    movss xmm1, xmm6
    mulss xmm1, DWORD PTR [mat+24]
    addss xmm0, xmm1
    addss xmm0, DWORD PTR [mat+28]
    movss DWORD PTR [dst+4], xmm0
    movss xmm0, xmm4
    mulss xmm0, DWORD PTR [mat+32]
    movss xmm1, xmm5
    mulss xmm1, DWORD PTR [mat+36]
    addss xmm0, xmm1
    movss xmm1, xmm6
    mulss xmm1, DWORD PTR [mat+40]
    addss xmm0, xmm1
    addss xmm0, DWORD PTR [mat+44]
    movss DWORD PTR [dst+8], xmm0
ENDM

; Perspective divide VEC4 into VEC3. Caller must handle w=0.
; First transforms src by mat, then computes x/w, y/w, z/w.
MAT4_PROJECT_POINT3 MACRO dst, mat, src
    LOCAL project_done
    MAT4_TRANSFORM4 dst, mat, src
    movss xmm1, DWORD PTR [dst+12]
    movss xmm0, DWORD PTR [dst+0]
    divss xmm0, xmm1
    movss DWORD PTR [dst+0], xmm0
    movss xmm0, DWORD PTR [dst+4]
    divss xmm0, xmm1
    movss DWORD PTR [dst+4], xmm0
    movss xmm0, DWORD PTR [dst+8]
    divss xmm0, xmm1
    movss DWORD PTR [dst+8], xmm0
ENDM

ENDIF ; UASM_TRANSFORM_INC_INCLUDED
