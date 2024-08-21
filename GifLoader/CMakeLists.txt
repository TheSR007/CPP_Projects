cmake_minimum_required(VERSION 3.20)
project(GifLoader VERSION 1.0 LANGUAGES CXX C)

find_package(OpenGL REQUIRED)

add_executable(GifLoader 
    main.cpp
)

target_compile_features(GifLoader PRIVATE cxx_std_20)
target_include_directories(GifLoader PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(LIBS 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui.lib
)

target_link_libraries(GifLoader PRIVATE 
    ${LIBS}
    OpenGL::GL
)
