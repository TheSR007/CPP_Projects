# KTXLoader Example for ImGui in OpenGL 

## This is an Example to Load ktx files in ImGui using OpenGL, Glfw and KTX which was supposed to be ported to Vulkan for TSM and SML

### Dependencies
- GLFW3
- ImGui
- OpenGL
- KTX

## Usage
You need the KTXLoader.exe and any ktx File
```cmd
KTXLoader path/to/ktx/name.ktx
```
or Just Run KTXLoader and then put the ktx path

## Buiding
ALL the lib are inside and Cmake is setup you just need to have OpenGL setup in your system so Cmake can find it using 
```Cmake 
find_package(OpenGL REQUIRED)
```
### CLI
Use VS build tool for CLI
```cmd
cd build
cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE="Release"
nmake
```
### Visual Studio
Import and normal Cmake Project building
