add_library(pd OBJECT Log.cpp)
target_link_libraries(pd glog::glog)