enable_testing()
set(GTEST_ROOT ${PROJECT_SOURCE_DIR}/opensource/googletest)
find_package(GTest REQUIRED)
include(${GTEST_ROOT}/lib/cmake/GTest/GTestConfig.cmake)
include(${PROJECT_SOURCE_DIR}/opensource/bson/FindBson.cmake)
include_directories(${GTEST_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/src ${bson_INCLUDE_DIRS})

add_subdirectory(st)
add_subdirectory(ut)