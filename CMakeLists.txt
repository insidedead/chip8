cmake_minimum_required(VERSION 2.6)

project(chip8)

find_package(SDL2 REQUIRED)

add_executable(main main.cpp)

add_library(cpu cpu.cpp cpu.h)
target_link_libraries(main PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(main PRIVATE cpu)
