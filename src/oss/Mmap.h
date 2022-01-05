#ifndef EMERALDDB_MMAP_H
#define EMERALDDB_MMAP_H

#include <vector>
#include <string>
#include "FileOperator.h"

namespace EMDB {
class Mmap {
protected:
    class MmapSegment;

public:
    Mmap() = default;
    ~Mmap() { Close(); }

    std::vector<MmapSegment>::const_iterator begin() const { return _segments.begin(); }
    std::vector<MmapSegment>::const_iterator end() const { return _segments.end(); }
    size_t size() { return _segments.size(); }

    int Open(const char* fileName, uint32_t fileMode);
    void Close();
    int Map(uint64_t offset, uint32_t length, void*& address);

private:
    FileOperator _fileOpr;
    bool _open{false};
    std::vector<MmapSegment> _segments;
    std::string _fileName{};
};

class Mmap::MmapSegment {
public:
    MmapSegment(void* ptr, uint32_t length, uint64_t offset) : _ptr(ptr), _length(length), _offset(offset) {}

    void *_ptr{nullptr};
    uint32_t _length{0};
    uint64_t _offset{0};
};

}  // namespace EMDB

#endif  // EMERALDDB_MMAP_H