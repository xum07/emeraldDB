aux_source_directory(. SOURCE)
add_library(client_cmd_test OBJECT ${SOURCE})
target_link_libraries(client_cmd_test PRIVATE client_cmd)