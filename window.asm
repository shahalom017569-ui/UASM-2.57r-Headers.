WINDOWS_AUTOLIB EQU 1
INCLUDE windows.inc

.code
start PROC
    INVOKE MessageBoxA, NULL, OFFSET msg, OFFSET title, MB_OK
    XOR eax, eax
    RET
start ENDP

.data
msg db "Hello from UASM!",0
title db "Demo",0

END start
