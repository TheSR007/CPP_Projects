cmake_minimum_required(VERSION 3.20)
project(KTXLoader VERSION 1.0 LANGUAGES CXX C)

find_package(OpenGL REQUIRED)

add_executable(KTXLoader 
    main.cpp
)

target_compile_features(KTXLoader PRIVATE cxx_std_20)
target_include_directories(KTXLoader PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(LIBS 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ktx.lib
)

target_link_libraries(KTXLoader PRIVATE 
    ${LIBS}
    OpenGL::GL
)
