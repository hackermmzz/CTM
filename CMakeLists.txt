cmake_minimum_required(VERSION 3.10)
project(CTM VERSION 1.0 LANGUAGES CXX)
file(GLOB SOURCES "./*.cpp")
add_executable(
                ctm 
                ${SOURCES}
                )