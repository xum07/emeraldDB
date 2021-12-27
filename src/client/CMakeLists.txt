add_subdirectory(cmd)

add_library(client STATIC
  Edb.cpp
)
target_link_libraries(client PRIVATE client_cmd)