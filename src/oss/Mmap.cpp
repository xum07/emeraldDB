#include "Mmap.h"
#include <sys/mman.h>
#include "ErrorCode.h"

using namespace EMDB;

int Mmap::Open(const char *fileName, uint32_t fileMode)
{
    auto ret = _fileOpr.Open(fileName, fileMode);
    if (ret != EDB_OK) {
        return ret;
    }

    _open = true;
    _fileName = fileName;
    return EDB_OK;
}

void Mmap::Close()
{
    for (const auto &seg : _segments) {
        munmap(seg._ptr, seg._length);
    }

    _segments.clear();
    if (_open) {
        _fileOpr.Close();
        _open = false;
    }
}

int Mmap::Map(uint64_t offset, uint32_t length, void*& address)
{
    if (length == 0) {
        return EDB_OK;
    }

    FileOperator::offsetType fileSize;
    auto ret = _fileOpr.GetSize(fileSize);
    if (ret != EDB_OK) {
        return ret;
    }

    if ((offset + length) > fileSize) {
        return EDB_INVALIDARG;
    }

    errno = 0;
    auto segment = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_SHARED, _fileOpr.GetHandle(), offset);
    if (segment == MAP_FAILED) {
        switch (errno) {
            case ENOMEM:
                return EDB_OOM;
            case EACCES:
                return EDB_PERM;
            default:
                return EDB_SYS;
        }
    }

    _segments.emplace_back({ segment, length, offset });
    if (address != nullptr) {
        address = segment;
    }
    return EDB_OK;
}