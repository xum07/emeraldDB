cmake_minimum_required(VERSION 3.14)
project(emeraldDB)

add_compile_options(-Wall)
set(CMAKE_CXX_FLAGS "-O0 -g -fPIC")

add_subdirectory(opensource)
add_subdirectory(src)
add_subdirectory(test)