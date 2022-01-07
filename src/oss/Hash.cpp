#include "Hash.h"

#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) || defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__TURBOC__)
#define GET_16_BITS(d) (*((const unsigned short *)(d)))
#else
#define GET_16_BITS(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8) + (uint32_t)(((const uint8_t *)(d))[0]))
#endif

namespace EMDB {
uint32_t CalHash(const char *data, int len)
{
    if (len <= 0 || data == nullptr) {
        return 0;
    }

    uint32_t hash = len;
    uint32_t tmp;
    auto rem = len & 3;
    len >>= 2;
    for (; len > 0; --len) {
        hash += GET_16_BITS(data);
        tmp = (GET_16_BITS(data + 2) << 11) ^ hash;
        hash = (hash << 16) ^ tmp;
        data += 2 * sizeof(unsigned short);
        hash += hash >> 11;
    }

    switch (rem) {
        case 3:
            hash += GET_16_BITS(data);
            hash ^= hash << 16;
            hash ^= ((char)data[sizeof(unsigned short)]) << 18;
            hash += hash >> 11;
            break;
        case 2:
            hash += GET_16_BITS(data);
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        case 1:
            hash += (char)*data;
            hash ^= hash << 10;
            hash += hash >> 1;
            break;
    }

    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    return hash;
}

}  // namespace EMDB