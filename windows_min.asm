WINDOWS_AUTOLIB EQU 1
INCLUDE windows_min.inc

.data
ClassName db "UASMWinClass",0
Title     db "Test",0
Msg       db "Hello",0

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

start PROC
    LOCAL wc:WNDCLASSEXA
    LOCAL msg:MSG

    mov wc.cbSize, SIZEOF WNDCLASSEXA
    mov wc.style, 0
    mov wc.lpfnWndProc, OFFSET WndProc
    mov wc.cbClsExtra, 0
    mov wc.cbWndExtra, 0
    mov wc.hInstance, NULL
    mov wc.hIcon, NULL
    mov wc.hCursor, NULL
    mov wc.hbrBackground, COLOR_WINDOW + 1
    mov wc.lpszMenuName, NULL
    lea eax, ClassName
    mov wc.lpszClassName, eax
    mov wc.hIconSm, NULL

    INVOKE RegisterClassExA, ADDR wc
    INVOKE CreateWindowExA, 0, ADDR ClassName, ADDR Title, WS_OVERLAPPEDWINDOW, \
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, NULL, NULL
    INVOKE MessageBoxA, NULL, ADDR Msg, ADDR Title, MB_OK
    INVOKE GetMessageA, ADDR msg, NULL, 0, 0
    INVOKE DispatchMessageA, ADDR msg
    XOR eax, eax
    RET
start ENDP
END start
