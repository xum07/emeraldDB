add_compile_options(-Werror)

include_directories(${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/fake ${bson_INCLUDE_DIRS})

add_subdirectory(client)
add_subdirectory(dms)
add_subdirectory(ixm)
add_subdirectory(mon)
add_subdirectory(msg)
add_subdirectory(oss)
add_subdirectory(pd)
add_subdirectory(pmd)
add_subdirectory(rtn)

add_executable(ut ut_main.cpp)
target_link_libraries(ut PRIVATE
  ${GTEST_BOTH_LIBRARIES}
  GTest::gmock
  GTest::gmock_main
  pthread
  oss_test
  client_cmd_test
)