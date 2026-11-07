; windows.inc - practical Windows API declarations for UASM 2.57r
; Supports 32-bit and 64-bit targets. This is a useful starter include,
; not a replacement for the Windows SDK or the full WinInc package.
;
; Architecture selection:
;   x86: default
;   x64: define WINDOWS_X64 before INCLUDE, or assemble with _WIN64 defined.
;
; Import/link conventions:
;   PROTO declarations below allow INVOKE and provide external procedure
;   declarations. Link the matching import libraries in the linker command,
;   or define WINDOWS_AUTOLIB before INCLUDE to emit INCLUDELIB directives
;   for the libraries covered here. You may instead add INCLUDELIB directives
;   in your source. Do not add a duplicate EXTERN for a PROTO-declared symbol.
;   A/W names are explicit: A uses the active ANSI code page; W uses UTF-16.
;   This file deliberately does not define the complete Windows SDK macro set.

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
COLORREF   TEXTEQU <DWORD>

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
HGDIOBJ    TEXTEQU <WIN_PTR>
HPEN       TEXTEQU <WIN_PTR>
HFONT      TEXTEQU <WIN_PTR>
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

; Common values and errors.
NULL                 EQU 0
FALSE                EQU 0
TRUE                 EQU 1
INVALID_HANDLE_VALUE EQU -1

; Window styles, extended styles, and messages.
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
WS_EX_TOPMOST       EQU 00000008h
WS_EX_TOOLWINDOW    EQU 00000080h

WM_NULL             EQU 0000h
WM_CREATE           EQU 0001h
WM_DESTROY          EQU 0002h
WM_MOVE             EQU 0003h
WM_SIZE             EQU 0005h
WM_SETFOCUS         EQU 0007h
WM_KILLFOCUS        EQU 0008h
WM_PAINT            EQU 000Fh
WM_CLOSE            EQU 0010h
WM_QUIT             EQU 0012h
WM_ERASEBKGND       EQU 0014h
WM_SETCURSOR        EQU 0020h
WM_COMMAND          EQU 0111h
WM_TIMER            EQU 0113h
WM_KEYDOWN          EQU 0100h
WM_KEYUP            EQU 0101h
WM_CHAR             EQU 0102h
WM_MOUSEMOVE        EQU 0200h
WM_LBUTTONDOWN      EQU 0201h
WM_LBUTTONUP        EQU 0202h
WM_RBUTTONDOWN      EQU 0204h
WM_RBUTTONUP        EQU 0205h
WM_USER             EQU 0400h

; MessageBox flags and ShowWindow commands.
MB_OK               EQU 00000000h
MB_OKCANCEL         EQU 00000001h
MB_YESNO            EQU 00000004h
MB_ICONERROR        EQU 00000010h
MB_ICONQUESTION     EQU 00000020h
MB_ICONWARNING      EQU 00000030h
MB_ICONINFORMATION  EQU 00000040h
SW_HIDE             EQU 0
SW_SHOWNORMAL       EQU 1
SW_SHOW             EQU 5

; File access, attributes, creation, and standard handle constants.
GENERIC_READ          EQU 80000000h
GENERIC_WRITE         EQU 40000000h
FILE_SHARE_READ       EQU 00000001h
FILE_SHARE_WRITE      EQU 00000002h
FILE_SHARE_DELETE     EQU 00000004h
CREATE_NEW            EQU 1
CREATE_ALWAYS         EQU 2
OPEN_EXISTING         EQU 3
OPEN_ALWAYS           EQU 4
TRUNCATE_EXISTING     EQU 5
FILE_ATTRIBUTE_READONLY EQU 00000001h
FILE_ATTRIBUTE_HIDDEN EQU 00000002h
FILE_ATTRIBUTE_NORMAL EQU 00000080h
FILE_BEGIN            EQU 0
FILE_CURRENT          EQU 1
FILE_END              EQU 2
STD_INPUT_HANDLE      EQU -10
STD_OUTPUT_HANDLE     EQU -11
STD_ERROR_HANDLE      EQU -12

; Memory, page protection, and wait constants.
MEM_COMMIT          EQU 00001000h
MEM_RESERVE         EQU 00002000h
MEM_DECOMMIT        EQU 00004000h
MEM_RELEASE         EQU 00008000h
PAGE_NOACCESS       EQU 00000001h
PAGE_READONLY       EQU 00000002h
PAGE_READWRITE      EQU 00000004h
PAGE_EXECUTE_READ   EQU 00000020h
INFINITE            EQU 0FFFFFFFFh
WAIT_OBJECT_0       EQU 0
WAIT_ABANDONED      EQU 00000080h
WAIT_TIMEOUT        EQU 00000102h
WAIT_FAILED         EQU 0FFFFFFFFh

; GDI, registry, and shell constants.
PS_SOLID            EQU 0
PS_DASH             EQU 1
TRANSPARENT         EQU 1
OPAQUE              EQU 2
SRCCOPY             EQU 00CC0020h
HKEY_CLASSES_ROOT   EQU 80000000h
HKEY_CURRENT_USER   EQU 80000001h
HKEY_LOCAL_MACHINE  EQU 80000002h
HKEY_USERS          EQU 80000003h
KEY_QUERY_VALUE     EQU 00000001h
KEY_SET_VALUE       EQU 00000002h
KEY_READ            EQU 00020019h
KEY_WRITE           EQU 00020006h
REG_NONE            EQU 0
REG_SZ              EQU 1
REG_EXPAND_SZ       EQU 2
REG_BINARY          EQU 3
REG_DWORD           EQU 4
REG_QWORD           EQU 11
SEE_MASK_NOCLOSEPROCESS EQU 00000040h

; Common structures.
POINT STRUCT
    x       SDWORD ?
    y       SDWORD ?
POINT ENDS

SIZE STRUCT
    cx      SDWORD ?
    cy      SDWORD ?
SIZE ENDS

RECT STRUCT
    left    SDWORD ?
    top     SDWORD ?
    right   SDWORD ?
    bottom  SDWORD ?
RECT ENDS

FILETIME STRUCT
    dwLowDateTime  DWORD ?
    dwHighDateTime DWORD ?
FILETIME ENDS

SYSTEMTIME STRUCT
    wYear         WORD ?
    wMonth        WORD ?
    wDayOfWeek    WORD ?
    wDay          WORD ?
    wHour         WORD ?
    wMinute       WORD ?
    wSecond       WORD ?
    wMilliseconds WORD ?
SYSTEMTIME ENDS

; OVERLAPPED is 20 bytes on Win32 and 32 bytes on Win64.
OVERLAPPED STRUCT
    Internal     WIN_UINTPTR ?
    InternalHigh WIN_UINTPTR ?
    Offset       DWORD ?
    OffsetHigh   DWORD ?
    hEvent       HANDLE ?
OVERLAPPED ENDS

; MSG layout is 28 bytes on Win32 and 48 bytes on Win64.
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

WNDCLASSEXA STRUCT
    cbSize          UINT ?
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
    hIconSm         HICON ?
WNDCLASSEXA ENDS

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

PAINTSTRUCT STRUCT
    hdc         HDC ?
    fErase      BOOL ?
IFDEF WINDOWS_X64
    _pad0       DWORD ?
ENDIF
    rcPaint     RECT <>
    fRestore    BOOL ?
    fIncUpdate  BOOL ?
    rgbReserved BYTE 32 DUP (?)
PAINTSTRUCT ENDS

; ShellExecuteExA structure, with Win64 alignment before pointer members.
SHELLEXECUTEINFOA STRUCT
    cbSize          DWORD ?
    fMask           DWORD ?
    hwnd            HWND ?
    lpVerb          LPCSTR ?
    lpFile          LPCSTR ?
    lpParameters    LPCSTR ?
    lpDirectory     LPCSTR ?
    nShow           INT ?
IFDEF WINDOWS_X64
    _pad0           DWORD ?
ENDIF
    hInstApp        HINSTANCE ?
    lpIDList        LPVOID ?
    lpClass         LPCSTR ?
    hkeyClass       HKEY ?
    dwHotKey        DWORD ?
IFDEF WINDOWS_X64
    _pad1           DWORD ?
ENDIF
    hIcon           HICON ?
SHELLEXECUTEINFOA ENDS

; WINAPI is stdcall on Win32 and the native x64 convention on Win64.
IFNDEF WINDOWS_X64
    WINAPI TEXTEQU < STDCALL>
ELSE
    WINAPI TEXTEQU <>
ENDIF

; Optional auto-link. Otherwise add required INCLUDELIB directives yourself.
IFDEF WINDOWS_AUTOLIB
    INCLUDELIB kernel32.lib
    INCLUDELIB user32.lib
    INCLUDELIB gdi32.lib
    INCLUDELIB advapi32.lib
    INCLUDELIB shell32.lib
ENDIF

; Kernel32: modules, process/thread, timing, environment, memory, files, console.
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
GetCurrentProcess      PROTO WINAPI
GetCurrentThread       PROTO WINAPI
Sleep                  PROTO WINAPI :DWORD
GetTickCount           PROTO WINAPI
GetTickCount64         PROTO WINAPI
GetSystemTime          PROTO WINAPI :PTR SYSTEMTIME
GetLocalTime           PROTO WINAPI :PTR SYSTEMTIME
GetSystemTimeAsFileTime PROTO WINAPI :PTR FILETIME
GetCommandLineA        PROTO WINAPI
GetCommandLineW        PROTO WINAPI
GetEnvironmentVariableA PROTO WINAPI :LPCSTR, :LPSTR, :DWORD
GetEnvironmentVariableW PROTO WINAPI :LPCWSTR, :LPWSTR, :DWORD
SetEnvironmentVariableA PROTO WINAPI :LPCSTR, :LPCSTR
SetEnvironmentVariableW PROTO WINAPI :LPCWSTR, :LPCWSTR

VirtualAlloc           PROTO WINAPI :LPVOID, :SIZE_T, :DWORD, :DWORD
VirtualFree            PROTO WINAPI :LPVOID, :SIZE_T, :DWORD
HeapAlloc              PROTO WINAPI :HANDLE, :DWORD, :SIZE_T
HeapFree               PROTO WINAPI :HANDLE, :DWORD, :LPVOID
GetProcessHeap         PROTO WINAPI

CreateFileA            PROTO WINAPI :LPCSTR, :DWORD, :DWORD, :PTR SECURITY_ATTRIBUTES, :DWORD, :DWORD, :HANDLE
CreateFileW            PROTO WINAPI :LPCWSTR, :DWORD, :DWORD, :PTR SECURITY_ATTRIBUTES, :DWORD, :DWORD, :HANDLE
ReadFile               PROTO WINAPI :HANDLE, :LPVOID, :DWORD, :PTR DWORD, :PTR OVERLAPPED
WriteFile              PROTO WINAPI :HANDLE, :LPCVOID, :DWORD, :PTR DWORD, :PTR OVERLAPPED
CloseHandle            PROTO WINAPI :HANDLE
GetFileSize            PROTO WINAPI :HANDLE, :PTR DWORD
GetFileSizeEx          PROTO WINAPI :HANDLE, :LPVOID
SetFilePointer         PROTO WINAPI :HANDLE, :LONG, :PTR LONG, :DWORD
FlushFileBuffers       PROTO WINAPI :HANDLE
DeleteFileA            PROTO WINAPI :LPCSTR
DeleteFileW            PROTO WINAPI :LPCWSTR
GetFileAttributesA     PROTO WINAPI :LPCSTR
GetFileAttributesW     PROTO WINAPI :LPCWSTR
CreateDirectoryA       PROTO WINAPI :LPCSTR, :PTR SECURITY_ATTRIBUTES
CreateDirectoryW       PROTO WINAPI :LPCWSTR, :PTR SECURITY_ATTRIBUTES
GetTempPathA           PROTO WINAPI :DWORD, :LPSTR
GetTempPathW           PROTO WINAPI :DWORD, :LPWSTR

GetStdHandle           PROTO WINAPI :DWORD
WriteConsoleA          PROTO WINAPI :HANDLE, :LPCVOID, :DWORD, :PTR DWORD, :LPVOID
WriteConsoleW          PROTO WINAPI :HANDLE, :LPCVOID, :DWORD, :PTR DWORD, :LPVOID
CreateProcessA         PROTO WINAPI :LPCSTR, :LPSTR, :PTR SECURITY_ATTRIBUTES, :PTR SECURITY_ATTRIBUTES, :BOOL, :DWORD, :LPVOID, :LPCSTR, :PTR STARTUPINFOA, :PTR PROCESS_INFORMATION
CreateProcessW         PROTO WINAPI :LPCWSTR, :LPWSTR, :PTR SECURITY_ATTRIBUTES, :PTR SECURITY_ATTRIBUTES, :BOOL, :DWORD, :LPVOID, :LPCWSTR, :PTR STARTUPINFOA, :PTR PROCESS_INFORMATION
WaitForSingleObject    PROTO WINAPI :HANDLE, :DWORD
TerminateProcess       PROTO WINAPI :HANDLE, :UINT

; User32: windows, painting, input, messages, clipboard.
MessageBoxA            PROTO WINAPI :HWND, :LPCSTR, :LPCSTR, :UINT
MessageBoxW            PROTO WINAPI :HWND, :LPCWSTR, :LPCWSTR, :UINT
RegisterClassA         PROTO WINAPI :PTR WNDCLASSA
RegisterClassExA       PROTO WINAPI :PTR WNDCLASSEXA
UnregisterClassA       PROTO WINAPI :LPCSTR, :HINSTANCE
CreateWindowExA        PROTO WINAPI :DWORD, :LPCSTR, :LPCSTR, :DWORD, :INT, :INT, :INT, :INT, :HWND, :HMENU, :HINSTANCE, :LPVOID
DefWindowProcA         PROTO WINAPI :HWND, :UINT, :WPARAM, :LPARAM
DestroyWindow          PROTO WINAPI :HWND
ShowWindow             PROTO WINAPI :HWND, :INT
UpdateWindow           PROTO WINAPI :HWND
SetWindowTextA         PROTO WINAPI :HWND, :LPCSTR
SetWindowTextW         PROTO WINAPI :HWND, :LPCWSTR
GetWindowTextA         PROTO WINAPI :HWND, :LPSTR, :INT
GetWindowTextW         PROTO WINAPI :HWND, :LPWSTR, :INT
GetClientRect          PROTO WINAPI :HWND, :PTR RECT
InvalidateRect         PROTO WINAPI :HWND, :PTR RECT, :BOOL
BeginPaint             PROTO WINAPI :HWND, :PTR PAINTSTRUCT
EndPaint               PROTO WINAPI :HWND, :PTR PAINTSTRUCT
GetMessageA            PROTO WINAPI :PTR MSG, :HWND, :UINT, :UINT
TranslateMessage       PROTO WINAPI :PTR MSG
DispatchMessageA       PROTO WINAPI :PTR MSG
PostMessageA           PROTO WINAPI :HWND, :UINT, :WPARAM, :LPARAM
PostQuitMessage        PROTO WINAPI :INT
SendMessageA           PROTO WINAPI :HWND, :UINT, :WPARAM, :LPARAM
GetDlgItem             PROTO WINAPI :HWND, :INT
SetTimer               PROTO WINAPI :HWND, :UINT_PTR, :UINT, :WIN_PTR
KillTimer              PROTO WINAPI :HWND, :UINT_PTR
GetCursorPos           PROTO WINAPI :PTR POINT
SetCursorPos           PROTO WINAPI :INT, :INT
GetAsyncKeyState       PROTO WINAPI :INT
MessageBeep            PROTO WINAPI :UINT

; GDI32: DCs, common drawing, fonts, text metrics, and bitmaps.
GetDC                  PROTO WINAPI :HWND
ReleaseDC              PROTO WINAPI :HWND, :HDC
CreateCompatibleDC     PROTO WINAPI :HDC
DeleteDC               PROTO WINAPI :HDC
CreateCompatibleBitmap PROTO WINAPI :HDC, :INT, :INT
CreatePen              PROTO WINAPI :INT, :INT, :COLORREF
CreateSolidBrush       PROTO WINAPI :COLORREF
CreateFontA            PROTO WINAPI :INT, :INT, :INT, :INT, :INT, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :LPCSTR
GetStockObject         PROTO WINAPI :INT
SelectObject           PROTO WINAPI :HDC, :HGDIOBJ
DeleteObject           PROTO WINAPI :HGDIOBJ
Rectangle              PROTO WINAPI :HDC, :INT, :INT, :INT, :INT
Ellipse                PROTO WINAPI :HDC, :INT, :INT, :INT, :INT
MoveToEx               PROTO WINAPI :HDC, :INT, :INT, :PTR POINT
LineTo                 PROTO WINAPI :HDC, :INT, :INT
SetPixel               PROTO WINAPI :HDC, :INT, :INT, :COLORREF
SetTextColor           PROTO WINAPI :HDC, :COLORREF
SetBkColor             PROTO WINAPI :HDC, :COLORREF
SetBkMode              PROTO WINAPI :HDC, :INT
TextOutA               PROTO WINAPI :HDC, :INT, :INT, :LPCSTR, :INT
TextOutW               PROTO WINAPI :HDC, :INT, :INT, :LPCWSTR, :INT
GetTextExtentPoint32A   PROTO WINAPI :HDC, :LPCSTR, :INT, :PTR SIZE
BitBlt                 PROTO WINAPI :HDC, :INT, :INT, :INT, :INT, :HDC, :INT, :INT, :DWORD

; Advapi32: registry and basic account/token helpers.
RegOpenKeyExA          PROTO WINAPI :HKEY, :LPCSTR, :DWORD, :DWORD, :PTR HKEY
RegOpenKeyExW          PROTO WINAPI :HKEY, :LPCWSTR, :DWORD, :DWORD, :PTR HKEY
RegCreateKeyExA        PROTO WINAPI :HKEY, :LPCSTR, :DWORD, :LPSTR, :DWORD, :DWORD, :PTR SECURITY_ATTRIBUTES, :PTR HKEY, :PTR DWORD
RegCreateKeyExW        PROTO WINAPI :HKEY, :LPCWSTR, :DWORD, :LPWSTR, :DWORD, :DWORD, :PTR SECURITY_ATTRIBUTES, :PTR HKEY, :PTR DWORD
RegQueryValueExA       PROTO WINAPI :HKEY, :LPCSTR, :PTR DWORD, :PTR DWORD, :LPVOID, :PTR DWORD
RegQueryValueExW       PROTO WINAPI :HKEY, :LPCWSTR, :PTR DWORD, :PTR DWORD, :LPVOID, :PTR DWORD
RegSetValueExA         PROTO WINAPI :HKEY, :LPCSTR, :DWORD, :DWORD, :LPCVOID, :DWORD
RegSetValueExW         PROTO WINAPI :HKEY, :LPCWSTR, :DWORD, :DWORD, :LPCVOID, :DWORD
RegDeleteValueA        PROTO WINAPI :HKEY, :LPCSTR
RegDeleteValueW        PROTO WINAPI :HKEY, :LPCWSTR
RegEnumKeyExA          PROTO WINAPI :HKEY, :DWORD, :LPSTR, :PTR DWORD, :PTR DWORD, :LPSTR, :PTR DWORD, :PTR FILETIME
RegEnumKeyExW          PROTO WINAPI :HKEY, :DWORD, :LPWSTR, :PTR DWORD, :PTR DWORD, :LPWSTR, :PTR DWORD, :PTR FILETIME
RegCloseKey            PROTO WINAPI :HKEY
RegFlushKey            PROTO WINAPI :HKEY
OpenProcessToken       PROTO WINAPI :HANDLE, :DWORD, :PTR HANDLE
GetUserNameA           PROTO WINAPI :LPSTR, :PTR DWORD
GetUserNameW           PROTO WINAPI :LPWSTR, :PTR DWORD

; Shell32: launching files/apps and retrieving common shell paths.
ShellExecuteA          PROTO WINAPI :HWND, :LPCSTR, :LPCSTR, :LPCSTR, :LPCSTR, :INT
ShellExecuteW          PROTO WINAPI :HWND, :LPCWSTR, :LPCWSTR, :LPCWSTR, :LPCWSTR, :INT
ShellExecuteExA        PROTO WINAPI :PTR SHELLEXECUTEINFOA
SHGetFolderPathA       PROTO WINAPI :HWND, :INT, :HANDLE, :DWORD, :LPSTR
SHGetFolderPathW       PROTO WINAPI :HWND, :INT, :HANDLE, :DWORD, :LPWSTR

ENDIF ; WINDOWS_INC_INCLUDED
