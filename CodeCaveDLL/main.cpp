//extern "C" void codecave();
#include <Windows.h>

__declspec(naked) void codecave();
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	DWORD old_protect;
	unsigned char* hook_location = (unsigned char*)0x00CCAF8A;

	if (fdwReason == DLL_PROCESS_ATTACH) {
		VirtualProtect((void*)hook_location, 6, PAGE_EXECUTE_READWRITE, &old_protect);
		*hook_location = 0xE9;
		*(SIZE_T*)(hook_location + 1) = (SIZE_T)&codecave - ((SIZE_T)hook_location + 5);
        *(hook_location + 5) = 0x58;
	}

	return true;
} 

void codecave(){
    asm(
        "movl $0x017EECB8 + 0x60, %eax\n"
        "movl (%eax), %eax\n"
        "addl $0xA90, %eax\n"
        "movl (%eax), %eax\n"
        "addl $4, %eax\n"
        "movl $999, (%eax)\n"
        "movl (%ecx), %eax\n"
        "leal (%esi), %esi\n"
        "pushl %eax\n"
        "movl $0xCCAF8F, %eax\n"
        "jmp *%eax"
    );

}



