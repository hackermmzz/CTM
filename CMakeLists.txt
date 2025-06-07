cmake_minimum_required(VERSION 3.10)
project(CTM VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
file(GLOB SOURCES "./*.cpp")
add_executable(
                ctm 
                ${SOURCES}
                )
if(MSVC)
    add_compile_options(/W0)
else()
    add_compile_options(-w)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Realse)
endif()