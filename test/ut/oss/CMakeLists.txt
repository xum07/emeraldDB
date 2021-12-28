aux_source_directory(. SOURCE)
add_library(oss_test OBJECT ${SOURCE})
target_link_libraries(oss_test PRIVATE oss)