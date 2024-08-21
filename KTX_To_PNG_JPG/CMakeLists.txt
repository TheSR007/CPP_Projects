cmake_minimum_required(VERSION 3.20)
project(KTXToPngJpg VERSION 1.0 LANGUAGES CXX)

find_package(OpenGL REQUIRED)

add_executable(KTXToPngJpg main.cpp)

target_compile_features(KTXToPngJpg PRIVATE cxx_std_20)
target_include_directories(KTXToPngJpg PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(LIBS 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ktx.lib
)

target_link_libraries(KTXToPngJpg PRIVATE 
    ${LIBS}
    OpenGL::GL
)
