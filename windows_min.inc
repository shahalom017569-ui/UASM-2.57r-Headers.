; windows_min.inc - minimal dual-mode UASM declarations
; x86 is the default. Define WINDOWS_X64 EQU 1 before INCLUDE for x64.
;
; Declares only common window/message APIs and the structures they need.
; Link user32.lib and kernel32.lib, or set WINDOWS_AUTOLIB EQU 1.
;
; Example:
;   WINDOWS_AUTOLIB EQU 1
;   INCLUDE windows_min.inc

IFNDEF WINDOWS_MIN_INC_INCLUDED
WINDOWS_MIN_INC_INCLUDED EQU 1

IFDEF _WIN64
    WINDOWS_X64 EQU 1
ENDIF

; Pointer-sized scalar and pointer aliases.
IFDEF WINDOWS_X64
    WIN_PTR      TEXTEQU <QWORD>
    WIN_INTPTR   TEXTEQU <QWORD>
    WIN_UINTPTR  TEXTEQU <QWORD>
    WINAPI       TEXTEQU <>
ELSE
    WIN_PTR      TEXTEQU <DWORD>
    WIN_INTPTR   TEXTEQU <SDWORD>
    WIN_UINTPTR  TEXTEQU <DWORD>
    WINAPI       TEXTEQU < STDCALL>
ENDIF

; Fixed-width Windows scalar types.
BOOL    TEXTEQU <SDWORD>
UINT    TEXTEQU <DWORD>
INT     TEXTEQU <SDWORD>
LONG    TEXTEQU <SDWORD>
WORD_T  TEXTEQU <WORD>
CHAR    TEXTEQU <BYTE>

; Handles, pointers, and callback types.
HANDLE      TEXTEQU <WIN_PTR>
HWND        TEXTEQU <WIN_PTR>
HINSTANCE   TEXTEQU <WIN_PTR>
HICON       TEXTEQU <WIN_PTR>
HCURSOR     TEXTEQU <WIN_PTR>
HBRUSH      TEXTEQU <WIN_PTR>
HMENU       TEXTEQU <WIN_PTR>
LPVOID      TEXTEQU <WIN_PTR>
LPCSTR      TEXTEQU <WIN_PTR>
LPSTR       TEXTEQU <WIN_PTR>
WPARAM      TEXTEQU <WIN_UINTPTR>
LPARAM      TEXTEQU <WIN_INTPTR>
LRESULT     TEXTEQU <WIN_INTPTR>
WNDPROC     TEXTEQU <WIN_PTR>

NULL            EQU 0
FALSE           EQU 0
TRUE            EQU 1

; Common styles and messages.
WS_OVERLAPPEDWINDOW EQU 00CF0000h
WS_VISIBLE          EQU 10000000h
WS_CHILD            EQU 40000000h

WM_DESTROY          EQU 0002h
WM_CLOSE            EQU 0010h
WM_QUIT             EQU 0012h

MB_OK               EQU 00000000h
MB_ICONINFORMATION  EQU 00000040h

SW_SHOWNORMAL       EQU 1
CW_USEDEFAULT       EQU 80000000h
COLOR_WINDOW        EQU 5

; Required structures.
POINT STRUCT
    x SDWORD ?
    y SDWORD ?
POINT ENDS

; MSG is 32 bytes on x86 and 48 bytes on x64.
MSG STRUCT
    hwnd    HWND ?
    message UINT ?
IFDEF WINDOWS_X64
    _pad0   DWORD ?
ENDIF
    wParam  WPARAM ?
    lParam  LPARAM ?
    time    DWORD ?
    pt      POINT <>
    lPrivate DWORD ?
MSG ENDS

; WNDCLASSEX has the same field order on both targets; pointer fields widen
; under x64, while the two initial DWORD fields place the first pointer at 8.
WNDCLASSEXA STRUCT
    cbSize        UINT ?
    style         UINT ?
    lpfnWndProc   WNDPROC ?
    cbClsExtra    INT ?
    cbWndExtra    INT ?
    hInstance     HINSTANCE ?
    hIcon         HICON ?
    hCursor       HCURSOR ?
    hbrBackground HBRUSH ?
    lpszMenuName  LPCSTR ?
    lpszClassName LPCSTR ?
    hIconSm       HICON ?
WNDCLASSEXA ENDS

; Optional import-library directives.
IFDEF WINDOWS_AUTOLIB
    INCLUDELIB kernel32.lib
    INCLUDELIB user32.lib
ENDIF

; Kernel32
GetModuleHandleA PROTO WINAPI :LPCSTR

; User32
MessageBoxA       PROTO WINAPI :HWND, :LPCSTR, :LPCSTR, :UINT
RegisterClassExA  PROTO WINAPI :PTR WNDCLASSEXA
CreateWindowExA   PROTO WINAPI :DWORD, :LPCSTR, :LPCSTR, :DWORD, :INT, :INT, :INT, :INT, :HWND, :HMENU, :HINSTANCE, :LPVOID
DefWindowProcA    PROTO WINAPI :HWND, :UINT, :WPARAM, :LPARAM
DestroyWindow     PROTO WINAPI :HWND
ShowWindow        PROTO WINAPI :HWND, :INT
UpdateWindow      PROTO WINAPI :HWND
GetMessageA       PROTO WINAPI :PTR MSG, :HWND, :UINT, :UINT
TranslateMessage  PROTO WINAPI :PTR MSG
DispatchMessageA  PROTO WINAPI :PTR MSG
PostQuitMessage   PROTO WINAPI :INT

ENDIF ; WINDOWS_MIN_INC_INCLUDED