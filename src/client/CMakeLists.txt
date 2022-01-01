add_subdirectory(cmd)

add_library(client OBJECT Client.cpp)
target_link_libraries(client PRIVATE client_cmd)

add_executable(${PROJECT_NAME}_client ClientMain.cpp)
target_link_libraries(${PROJECT_NAME}_client PRIVATE client client_cmd)