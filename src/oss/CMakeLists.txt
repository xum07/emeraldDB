add_library(oss OBJECT
  Socket.cpp
)
target_link_libraries(oss PRIVATE pd)