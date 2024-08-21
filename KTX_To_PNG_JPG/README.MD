# KTX to PNG and JPG

### Dependencies
- GLFW3
- OpenGL
- STB_Image_Write

## Usage
You need the KTXToPngJpg.exe and any ktx
```cmd
KTXToPngJpg path/to/ktxfolder
```
or Just Run KTXToPngJpg and then put the ktx folder path

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
