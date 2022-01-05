project(emeraldDB)
cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -fPIC")
add_compile_options(-Wall -fstack-protector -Wl,-z,noexecstack)
add_compile_definitions(_FILE_OFFSET_BITS=64)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
include_directories(include)

add_subdirectory(opensource)
add_subdirectory(src)
add_subdirectory(test)

