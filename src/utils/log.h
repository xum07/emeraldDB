#ifndef EMERALDDB_LOG_H
#define EMERALDDB_LOG_H

#include <iostream>

#define LOCATION "[" << __FILE__ << ":" << __LINE__ << "]"
#define LOG_IF(condition)
#define LOG std::cout << LOCATION

#endif //EMERALDDB_LOG_H
