cmake_minimum_required(VERSION 3.28)
project("KittnusOS")

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "-Wall -Werror -m64 -mabi=ms")

set(CMAKE_CXX_COMPILER "g++")

add_subdirectory(Source/Boot)
add_subdirectory(Source/Kernel)
