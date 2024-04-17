The default main.cpp uses GCC/CLANG inline assembly code
g++ -shared -o main.dll main.cpp
to build the .dll

you can use VS compiler for __asm{ } formal assmebly
but if you want to use GCC/CLANG but not GCC/CLANG inline
Assembly code
you can use nasm/yasm assembly syntax
then uncomment the extern "C" void codecave();
and comment out the void codecave() function inside the main.cpp
the codecave function is defined in the assembly.asm
nasm -f win32 assembly.asm -o assembly.obj
(yasm can be used too)
g++ -shared -o main.dll main.cpp assembly.obj

-m32 if using 64bit g++


