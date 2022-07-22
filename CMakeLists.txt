cmake_minimum_required(VERSION 3.22)
project(GBE)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(src/)

add_executable(GBE main.cpp src/RAM/RAM.cpp src/RAM/RAM.h src/CPU/CPU.cpp src/CPU/CPU.h src/CPU/Disasm.h src/CPU/Disasm.cpp src/Display/Display.cpp src/Display/Display.h)
target_link_libraries(GBE ${SDL2_LIBRARIES})
