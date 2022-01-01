project(EmeraldDB)
cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -fPIC")
add_compile_options(-Wall -fstack-protector -Wl,-z,noexecstack)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/opensource)

add_subdirectory(opensource)
add_subdirectory(src)
add_subdirectory(test)

