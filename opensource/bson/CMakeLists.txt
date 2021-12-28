cmake_minimum_required(VERSION 2.8)

add_library(bson STATIC
  src/bsonobj.cpp
  src/oid.cpp
  src/util/json.cpp
  lib/base64.cpp
  lib/md5.cpp
  lib/nonce.cpp
)
target_include_directories(bson PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(bson PRIVATE boost_thread pthread)