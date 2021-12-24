aux_source_directory(. SOURCE)
add_library(oss_test OBJECT ${SOURCE})
target_include_directories(oss_test PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(oss_test PRIVATE oss)