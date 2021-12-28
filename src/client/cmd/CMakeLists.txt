add_library(client_cmd STATIC
  CmdInf.cpp
  HelpCmd.cpp
  QuitCmd.cpp
  ConnectCmd.cpp
)
target_link_libraries(client_cmd PRIVATE utils bson)
target_include_directories(client_cmd PRIVATE ${bson_INCLUDE_DIRS})