#include "FileOperator.h"
#include <sys/types.h>
#include <fcntl.h>
#include <cerrno>
#include "ErrorCode.h"

using namespace EMDB;

#define FILE_NOT_EXIST(fResult) ((fResult == -1) && (errno == EINTR))

int FileOperator::Open(const char *filePath, uint32_t flags)
{
    int flag = O_RDWR;
    if (flags & OSS_FILE_READ_ONLY) {
        flag = O_RDONLY;
    } else if (flags & OSS_FILE_WRITE_ONLY) {
        flag = O_WRONLY;
    }

    if (flags & OSS_FILE_OPEN_ALWAYS) {
        flag |= O_CREAT;
    }
    if (flags & OSS_FILE_OPEN_TRUNCATE) {
        flag |= O_TRUNC;
    }

    errno = 0;
    do {
        _fd = oss_open(filePath, flag, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    } while (FILE_NOT_EXIST(_fd));

    if (_fd <= OSS_INVALID_FD) {
        return errno;
    }

    return EDB_OK;
}

int FileOperator::Read(void *buff, size_t buffSize, ssize_t &readBytes)
{
    readBytes = 0;
    if (!IsValid()) {
        return EDB_ERR;
    }

    errno = 0;
    ssize_t ret;
    do {
        ret = oss_read(_fd, buff, buffSize);
    } while (FILE_NOT_EXIST(ret));
    if (ret == -1) {
        return errno;
    }

    readBytes = ret;
    return EDB_OK;
}

int FileOperator::Write(const char *buff, size_t buffSize)
{
    if (!IsValid()) {
        return EDB_OK;
    }

    errno = 0;
    int ret;
    size_t curSize = 0;
    do {
        ret = oss_write(_fd, &(buff)[curSize], buffSize - curSize);
        if (ret >= 0) {
            curSize += ret;
        }
    } while (FILE_NOT_EXIST(ret) || ((ret != -1) && (curSize != buffSize)));
    if (ret == -1) {
        return errno;
    }

    return EDB_OK;
}

int FileOperator::GetSize(offsetType &fileSize)
{
    oss_struct_stat buf = {0};
    errno = 0;
    if (oss_fstat(_fd, &buf) == -1) {
        fileSize = 0;
        return errno;
    }

    fileSize = buf.st_size;
    return EDB_OK;
}
