aux_source_directory(. SOURCE)
add_library(client_cmd_test STATIC ${SOURCE})
target_include_directories(client_cmd_test PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(client_cmd_test PRIVATE client_cmd)