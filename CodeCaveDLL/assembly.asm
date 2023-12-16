section .text
global _codecave

_codecave:
    mov eax, 0x017EECB8 + 0x60
    mov eax, [eax]
    add eax, 0xA90
    mov eax, [eax]
    add eax, 4
    mov dword [eax], 999
    mov eax, [ecx]
	lea esi, [esi]
    push eax 
    mov eax, 0xCCAF8F
    jmp eax
