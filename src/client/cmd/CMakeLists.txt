add_library(client_cmd STATIC
  CmdInf.cpp
  HelpCmd.cpp
  QuitCmd.cpp
  ConnectCmd.cpp
)
target_link_libraries(client_cmd PRIVATE utils)
target_include_directories(client_cmd PRIVATE ${PROJECT_SOURCE_DIR}/opensource)