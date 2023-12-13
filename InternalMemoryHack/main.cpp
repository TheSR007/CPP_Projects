#include <Windows.h>

__attribute__((stdcall)) void injected_thread();

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved ) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)injected_thread, NULL, 0, NULL);
    }

    return true;
}

__attribute__((stdcall)) void injected_thread() {
    while (true) {
    if (GetAsyncKeyState('M')) {
        SIZE_T* Player_Base_Address = (SIZE_T*)(0x017EECB8 + 0x60);
        SIZE_T* Game_Base_Address = (SIZE_T*)(*Player_Base_Address + 0xA90);
        SIZE_T* Gold_Address = (SIZE_T*)(*Game_Base_Address + 4);
        *Gold_Address = 999;    
    }

    Sleep(1);
    }
}