#include "pch.h"

void Patch(BYTE* dst, BYTE* src, unsigned int size)
{
    if (src == nullptr)
        return;
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

bool Hook(char* src, char* dst, int len) {
    if (len < 5) return false;
    DWORD oProc;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oProc);
    memset(src, 0x90, len);
    uintptr_t relativeAddress = (uintptr_t)(dst - src - 5);
    *src = (char)0xE9;
    *(uintptr_t*)(src + 1) = (uintptr_t)relativeAddress;
    VirtualProtect(src, len, oProc, &oProc);

    return true;
}

char* TrampHook(char* src, char* dst, unsigned int len) {
    if (len < 5) return 0;
    char* gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(gateway, src, len);
    uintptr_t jumpAddr = (uintptr_t)(src - gateway - 5);
    *(gateway + len) = (char)0xE9;
    *(uintptr_t*)(gateway + len + 1) = jumpAddr;
    if (Hook(src, dst, len)) {
        return gateway;
    }
    else return nullptr;
}