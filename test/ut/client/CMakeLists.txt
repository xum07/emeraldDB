add_subdirectory(cmd)

aux_source_directory(. SOURCE)
add_library(client_test OBJECT ${SOURCE})
target_link_libraries(client_test PRIVATE client)