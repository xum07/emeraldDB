find_path(bson_ROOT src/bson.h HINTS ${PROJECT_SOURCE_DIR}/opensource/bson)
set(bson_INCLUDE_DIRS ${bson_ROOT} ${bson_ROOT}/src ${bson_ROOT}/lib)