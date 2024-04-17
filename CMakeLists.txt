cmake_minimum_required(VERSION 3.24)
project(automata)
set(CMAKE_C_STANDARD 99)
# set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(automata src/main.c)
