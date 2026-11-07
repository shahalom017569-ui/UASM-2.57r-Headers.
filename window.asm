; real_window.asm
; Minimal actual Win32 window for UASM 2.57r

WINDOWS_AUTOLIB EQU 1
INCLUDE windows.inc

CW_USEDEFAULT EQU 80000000h
CS_HREDRAW    EQU 0002h
CS_VREDRAW    EQU 0001h
IDC_ARROW     EQU 32512
IDI_APPLICATION EQU 32512

.data
ClassName db "UASMWinClass",0
WindowTitle db "UASM Real Window",0

.code
WndProc PROC hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
    .IF uMsg == WM_DESTROY
        INVOKE PostQuitMessage, 0
        XOR eax, eax
        RET
    .ELSE
        INVOKE DefWindowProcA, hWnd, uMsg, wParam, lParam
        RET
    .ENDIF
WndProc ENDP

WinMain PROC hInstance:HINSTANCE, hPrevInstance:HINSTANCE, lpCmdLine:LPSTR, nShow:INT
    LOCAL wc:WNDCLASSEXA
    LOCAL hwnd:HWND
    LOCAL msg:MSG

    ; Fill WNDCLASSEXA
    mov wc.cbSize, SIZEOF WNDCLASSEXA
    mov wc.style, CS_HREDRAW OR CS_VREDRAW
    mov wc.lpfnWndProc, OFFSET WndProc
    mov wc.cbClsExtra, 0
    mov wc.cbWndExtra, 0
    mov wc.hInstance, hInstance
    mov wc.hIcon, NULL
    mov wc.hCursor, NULL
    mov wc.hbrBackground, COLOR_WINDOW + 1
    mov wc.lpszMenuName, NULL
    lea eax, ClassName
    mov wc.lpszClassName, eax
    mov wc.hIconSm, NULL

    ; Register window class
    INVOKE RegisterClassExA, ADDR wc

    ; Create actual window
    INVOKE CreateWindowExA, 0, \
        ADDR ClassName, \
        ADDR WindowTitle, \
        WS_OVERLAPPEDWINDOW, \
        CW_USEDEFAULT, CW_USEDEFAULT, \
        640, 480, \
        NULL, NULL, hInstance, NULL

    mov hwnd, eax
    .IF hwnd == NULL
        XOR eax, eax
        RET
    .ENDIF

    ; Show and update window
    INVOKE ShowWindow, hwnd, nShow
    INVOKE UpdateWindow, hwnd

    ; Message loop
    .WHILE TRUE
        INVOKE GetMessageA, ADDR msg, NULL, 0, 0
        .IF eax == 0
            ; WM_QUIT
            .BREAK
        .ENDIF
        INVOKE TranslateMessage, ADDR msg
        INVOKE DispatchMessageA, ADDR msg
    .ENDW

    mov eax, msg.wParam
    RET
WinMain ENDP

start PROC
    INVOKE GetModuleHandleA, NULL
    INVOKE WinMain, eax, NULL, NULL, SW_SHOWNORMAL
    RET
start ENDP

END start
