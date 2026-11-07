; windows.inc - compact Windows API declarations for UASM 2.57r
; Supports 32-bit and 64-bit targets. This is a practical starter include,
; not a replacement for the Windows SDK or the full WinInc package.
;
; Architecture selection:
;   x86: default
;   x64: define WINDOWS_X64 before including this file, or assemble with
;        _WIN64 already defined.
;
; API prototypes are declarations only. Link the corresponding Windows import
; libraries (commonly kernel32.lib and user32.lib).

IFNDEF WINDOWS_INC_INCLUDED
WINDOWS_INC_INCLUDED EQU 1

IFDEF _WIN64
    WINDOWS_X64 EQU 1
ENDIF

IFDEF WINDOWS_X64
    WIN_PTR       TEXTEQU <QWORD>
    WIN_INTPTR    TEXTEQU <QWORD>
    WIN_UINTPTR   TEXTEQU <QWORD>
    WIN_SIZE_T    TEXTEQU <QWORD>
    WIN_LONGPTR   TEXTEQU <QWORD>
    WIN_ULONGPTR  TEXTEQU <QWORD>
ELSE
    WIN_PTR       TEXTEQU <DWORD>
    WIN_INTPTR    TEXTEQU <SDWORD>
    WIN_UINTPTR   TEXTEQU <DWORD>
    WIN_SIZE_T    TEXTEQU <DWORD>
    WIN_LONGPTR   TEXTEQU <SDWORD>
    WIN_ULONGPTR  TEXTEQU <DWORD>
ENDIF

; Fundamental Windows scalar and pointer types.
BOOL       TEXTEQU <SDWORD>
BOOLEAN    TEXTEQU <BYTE>
UINT       TEXTEQU <DWORD>
INT        TEXTEQU <SDWORD>
LONG       TEXTEQU <SDWORD>
ULONG      TEXTEQU <DWORD>
SHORT      TEXTEQU <SWORD>
USHORT     TEXTEQU <WORD>
CHAR       TEXTEQU <BYTE>
WCHAR      TEXTEQU <WORD>
FLOAT_T    TEXTEQU <REAL4>

HANDLE     TEXTEQU <WIN_PTR>
HWND       TEXTEQU <WIN_PTR>
HINSTANCE  TEXTEQU <WIN_PTR>
HICON      TEXTEQU <WIN_PTR>
HCURSOR    TEXTEQU <WIN_PTR>
HBRUSH     TEXTEQU <WIN_PTR>
HMENU      TEXTEQU <WIN_PTR>
HDC        TEXTEQU <WIN_PTR>
HBITMAP    TEXTEQU <WIN_PTR>
HMODULE    TEXTEQU <WIN_PTR>
HKEY       TEXTEQU <WIN_PTR>
HGLOBAL    TEXTEQU <WIN_PTR>
HLOCAL     TEXTEQU <WIN_PTR>
FARPROC    TEXTEQU <WIN_PTR>
LPVOID     TEXTEQU <WIN_PTR>
LPCVOID    TEXTEQU <WIN_PTR>
LPSTR      TEXTEQU <WIN_PTR>
LPCSTR     TEXTEQU <WIN_PTR>
LPWSTR     TEXTEQU <WIN_PTR>
LPCWSTR    TEXTEQU <WIN_PTR>
LRESULT    TEXTEQU <WIN_INTPTR>
WPARAM     TEXTEQU <WIN_UINTPTR>
LPARAM     TEXTEQU <WIN_INTPTR>
UINT_PTR   TEXTEQU <WIN_UINTPTR>
INT_PTR    TEXTEQU <WIN_INTPTR>
LONG_PTR   TEXTEQU <WIN_LONGPTR>
ULONG_PTR  TEXTEQU <WIN_ULONGPTR>
SIZE_T     TEXTEQU <WIN_SIZE_T>

; Common constants.
NULL                EQU 0
FALSE               EQU 0
TRUE                EQU 1
INVALID_HANDLE_VALUE EQU -1

; Window styles and messages.
WS_OVERLAPPED       EQU 00000000h
WS_POPUP            EQU 80000000h
WS_CHILD            EQU 40000000h
WS_MINIMIZE         EQU 20000000h
WS_VISIBLE          EQU 10000000h
WS_DISABLED         EQU 08000000h
WS_CLIPSIBLINGS     EQU 04000000h
WS_CLIPCHILDREN     EQU 02000000h
WS_MAXIMIZE         EQU 01000000h
WS_CAPTION          EQU 00C00000h
WS_BORDER           EQU 00800000h
WS_DLGFRAME         EQU 00400000h
WS_VSCROLL          EQU 00200000h
WS_HSCROLL          EQU 00100000h
WS_SYSMENU          EQU 00080000h
WS_THICKFRAME       EQU 00040000h
WS_GROUP            EQU 00020000h
WS_TABSTOP          EQU 00010000h
WS_OVERLAPPEDWINDOW EQU 00CF0000h
WS_EX_APPWINDOW     EQU 00040000h

WM_NULL             EQU 0000h
WM_CREATE           EQU 0001h
WM_DESTROY          EQU 0002h
WM_MOVE             EQU 0003h
WM_SIZE             EQU 0005h
WM_CLOSE            EQU 0010h
WM_QUIT             EQU 0012h
WM_KEYDOWN          EQU 0100h
WM_KEYUP            EQU 0101h
WM_COMMAND          EQU 0111h
WM_USER             EQU 0400h

; MessageBox flags and ShowWindow commands.
MB_OK               EQU 00000000h
MB_OKCANCEL         EQU 00000001h
MB_ICONERROR        EQU 00000010h
MB_ICONQUESTION     EQU 00000020h
MB_ICONWARNING      EQU 00000030h
MB_ICONINFORMATION  EQU 00000040h
SW_HIDE             EQU 0
SW_SHOWNORMAL       EQU 1
SW_SHOW             EQU 5

; File access, creation, and standard handle constants.
GENERIC_READ        EQU 80000000h
GENERIC_WRITE       EQU 40000000h
FILE_SHARE_READ     EQU 00000001h
FILE_SHARE_WRITE    EQU 00000002h
CREATE_NEW          EQU 1
CREATE_ALWAYS       EQU 2
OPEN_EXISTING       EQU 3
OPEN_ALWAYS         EQU 4
TRUNCATE_EXISTING   EQU 5
FILE_ATTRIBUTE_NORMAL EQU 00000080h
STD_INPUT_HANDLE    EQU -10
STD_OUTPUT_HANDLE   EQU -11
STD_ERROR_HANDLE    EQU -12

; Memory and wait constants.
MEM_COMMIT          EQU 00001000h
MEM_RESERVE         EQU 00002000h
MEM_RELEASE         EQU 00008000h
PAGE_READWRITE      EQU 00000004h
PAGE_EXECUTE_READ   EQU 00000020h
INFINITE            EQU 0FFFFFFFFh
WAIT_OBJECT_0       EQU 0
WAIT_TIMEOUT        EQU 00000102h
WAIT_FAILED         EQU 0FFFFFFFFh

; Basic, architecture-independent structures.
POINT STRUCT
    x       SDWORD ?
    y       SDWORD ?
POINT ENDS

RECT STRUCT
    left    SDWORD ?
    top     SDWORD ?
    right   SDWORD ?
    bottom  SDWORD ?
RECT ENDS

; MSG layout is 48 bytes on Win64 and 28 bytes on Win32.
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
IFDEF WINDOWS_X64
    _pad1   DWORD ?
ENDIF
MSG ENDS

; WNDCLASSA layout is 72 bytes on Win64 and 40 bytes on Win32.
WNDCLASSA STRUCT
    style           UINT ?
IFDEF WINDOWS_X64
    _pad0           DWORD ?
ENDIF
    lpfnWndProc     WIN_PTR ?
    cbClsExtra      INT ?
    cbWndExtra      INT ?
    hInstance       HINSTANCE ?
    hIcon           HICON ?
    hCursor         HCURSOR ?
    hbrBackground   HBRUSH ?
    lpszMenuName    LPCSTR ?
    lpszClassName   LPCSTR ?
WNDCLASSA ENDS

; Process startup structures, with explicit Win64 alignment padding.
SECURITY_ATTRIBUTES STRUCT
    nLength             DWORD ?
IFDEF WINDOWS_X64
    _pad0               DWORD ?
ENDIF
    lpSecurityDescriptor LPVOID ?
    bInheritHandle      BOOL ?
IFDEF WINDOWS_X64
    _pad1               DWORD ?
ENDIF
SECURITY_ATTRIBUTES ENDS

STARTUPINFOA STRUCT
    cb              DWORD ?
IFDEF WINDOWS_X64
    _pad0           DWORD ?
ENDIF
    lpReserved      LPSTR ?
    lpDesktop       LPSTR ?
    lpTitle         LPSTR ?
    dwX             DWORD ?
    dwY             DWORD ?
    dwXSize         DWORD ?
    dwYSize         DWORD ?
    dwXCountChars   DWORD ?
    dwYCountChars   DWORD ?
    dwFillAttribute DWORD ?
    dwFlags         DWORD ?
    wShowWindow     WORD ?
    cbReserved2     WORD ?
IFDEF WINDOWS_X64
    _pad1           DWORD ?
ENDIF
    lpReserved2     LPVOID ?
    hStdInput       HANDLE ?
    hStdOutput      HANDLE ?
    hStdError       HANDLE ?
STARTUPINFOA ENDS

PROCESS_INFORMATION STRUCT
    hProcess        HANDLE ?
    hThread         HANDLE ?
    dwProcessId     DWORD ?
    dwThreadId      DWORD ?
IFDEF WINDOWS_X64
    _pad0           DWORD ?
ENDIF
PROCESS_INFORMATION ENDS

; WINAPI is stdcall on Win32 and the native x64 convention on Win64.
IFNDEF WINDOWS_X64
    WINAPI TEXTEQU < STDCALL>
ELSE
    WINAPI TEXTEQU <>
ENDIF

; Kernel32: modules, process/thread control, memory, files, console, time.
GetModuleHandleA       PROTO WINAPI :LPCSTR
GetModuleHandleW       PROTO WINAPI :LPCWSTR
LoadLibraryA           PROTO WINAPI :LPCSTR
LoadLibraryW           PROTO WINAPI :LPCWSTR
GetProcAddress         PROTO WINAPI :HMODULE, :LPCSTR
FreeLibrary            PROTO WINAPI :HMODULE
GetLastError           PROTO WINAPI
SetLastError           PROTO WINAPI :DWORD
ExitProcess            PROTO WINAPI :UINT
GetCurrentProcessId    PROTO WINAPI
GetCurrentThreadId     PROTO WINAPI
Sleep                  PROTO WINAPI :DWORD
GetTickCount           PROTO WINAPI
GetTickCount64         PROTO WINAPI

VirtualAlloc           PROTO WINAPI :LPVOID, :SIZE_T, :DWORD, :DWORD
VirtualFree            PROTO WINAPI :LPVOID, :SIZE_T, :DWORD
HeapAlloc              PROTO WINAPI :HANDLE, :DWORD, :SIZE_T
HeapFree               PROTO WINAPI :HANDLE, :DWORD, :LPVOID
GetProcessHeap         PROTO WINAPI

CreateFileA            PROTO WINAPI :LPCSTR, :DWORD, :DWORD, :PTR SECURITY_ATTRIBUTES, :DWORD, :DWORD, :HANDLE
CreateFileW            PROTO WINAPI :LPCWSTR, :DWORD, :DWORD, :PTR SECURITY_ATTRIBUTES, :DWORD, :DWORD, :HANDLE
ReadFile               PROTO WINAPI :HANDLE, :LPVOID, :DWORD, :LPVOID, :LPVOID
WriteFile              PROTO WINAPI :HANDLE, :LPCVOID, :DWORD, :LPVOID, :LPVOID
CloseHandle            PROTO WINAPI :HANDLE
GetFileSize            PROTO WINAPI :HANDLE, :LPVOID
DeleteFileA            PROTO WINAPI :LPCSTR
DeleteFileW            PROTO WINAPI :LPCWSTR

GetStdHandle           PROTO WINAPI :DWORD
WriteConsoleA          PROTO WINAPI :HANDLE, :LPCVOID, :DWORD, :LPVOID, :LPVOID
WriteConsoleW          PROTO WINAPI :HANDLE, :LPCVOID, :DWORD, :LPVOID, :LPVOID

CreateProcessA         PROTO WINAPI :LPCSTR, :LPSTR, :PTR SECURITY_ATTRIBUTES, :PTR SECURITY_ATTRIBUTES, :BOOL, :DWORD, :LPVOID, :LPCSTR, :PTR STARTUPINFOA, :PTR PROCESS_INFORMATION
CreateProcessW         PROTO WINAPI :LPCWSTR, :LPWSTR, :PTR SECURITY_ATTRIBUTES, :PTR SECURITY_ATTRIBUTES, :BOOL, :DWORD, :LPVOID, :LPCWSTR, :PTR STARTUPINFOA, :PTR PROCESS_INFORMATION
WaitForSingleObject    PROTO WINAPI :HANDLE, :DWORD
TerminateProcess       PROTO WINAPI :HANDLE, :UINT

; User32: message boxes, windows, and message loop.
MessageBoxA            PROTO WINAPI :HWND, :LPCSTR, :LPCSTR, :UINT
MessageBoxW            PROTO WINAPI :HWND, :LPCWSTR, :LPCWSTR, :UINT
RegisterClassA         PROTO WINAPI :PTR WNDCLASSA
CreateWindowExA        PROTO WINAPI :DWORD, :LPCSTR, :LPCSTR, :DWORD, :INT, :INT, :INT, :INT, :HWND, :HMENU, :HINSTANCE, :LPVOID
DefWindowProcA         PROTO WINAPI :HWND, :UINT, :WPARAM, :LPARAM
ShowWindow             PROTO WINAPI :HWND, :INT
UpdateWindow           PROTO WINAPI :HWND
GetMessageA            PROTO WINAPI :PTR MSG, :HWND, :UINT, :UINT
TranslateMessage       PROTO WINAPI :PTR MSG
DispatchMessageA       PROTO WINAPI :PTR MSG
PostQuitMessage        PROTO WINAPI :INT

ENDIF ; WINDOWS_INC_INCLUDED
