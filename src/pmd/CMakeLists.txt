add_library(server OBJECT TcpListener.cpp)
target_link_libraries(server PRIVATE pd)

add_executable(${PROJECT_NAME}_server
  ServerMain.cpp
)
target_link_libraries(${PROJECT_NAME}_server PRIVATE server pd oss)
