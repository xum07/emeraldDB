add_library(client_cmd OBJECT
  CmdInf.cpp
  TestCmd.cpp
  HelpCmd.cpp
  QuitCmd.cpp
  ConnectCmd.cpp
)
target_link_libraries(client_cmd PRIVATE utils pd bson)
target_include_directories(client_cmd PUBLIC ${bson_INCLUDE_DIRS})