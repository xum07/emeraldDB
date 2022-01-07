#ifndef EMERALDDB_HASH_H
#define EMERALDDB_HASH_H

#include <cstdint>

namespace EMDB {
uint32_t CalHash(const char *data, int len);

}  // namespace EMDB

#endif  // EMERALDDB_HASH_H