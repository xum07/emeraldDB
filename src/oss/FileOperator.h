#ifndef EMERALDDB_FILE_OPERATOR_H
#define EMERALDDB_FILE_OPERATOR_H

#include <cstddef>
#include <cstdint>
#include <sys/stat.h>
#include <cstdio>

#define OSS_F_GETLK        F_GETLK64
#define OSS_F_SETLK        F_SETLK64
#define OSS_F_SETLKW       F_SETLKW64

#define oss_struct_statfs  struct statfs64
#define oss_statfs         statfs64
#define oss_fstatfs        fstatfs64
#define oss_struct_statvfs struct statvfs64
#define oss_statvfs        statvfs64
#define oss_fstatvfs       fstatvfs64
#define oss_struct_stat    struct stat64
#define oss_struct_flock   struct flock64
#define oss_stat           stat64
#define oss_lstat          lstat64
#define oss_fstat          fstat64
#define oss_open           open64
#define oss_lseek          lseek64
#define oss_ftruncate      ftruncate64
#define oss_off_t          off64_t
#define oss_close          close
#define oss_access         access
#define oss_chmod          chmod
#define oss_read           read
#define oss_write          write

namespace EMDB {

constexpr uint32_t OSS_FILE_READ_ONLY = 0b1;
constexpr uint32_t OSS_FILE_WRITE_ONLY = 0b10;
constexpr uint32_t OSS_FILE_OPEN_EXISTING = 0b100;
constexpr uint32_t OSS_FILE_OPEN_ALWAYS = 0b1000;
constexpr uint32_t OSS_FILE_OPEN_TRUNCATE = 0b10000;
constexpr int OSS_INVALID_FD = -1;

class FileOperator {
public:
    using offsetType = oss_off_t;

    FileOperator() = default;
    ~FileOperator() = default;
    FileOperator(const FileOperator &) = delete;
    const FileOperator &operator=(const FileOperator &) = delete;

    int Open(const char *filePath, uint32_t flags = OSS_FILE_OPEN_ALWAYS);
    void OpenStdout()
    {
        SetFileDescriptor(STDOUT_FILENO);
        _stdout = true;
    };

    void Close()
    {
        if (IsValid() && (!_stdout)) {
            oss_close(_fd);
            _fd = OSS_INVALID_FD;
        }
    };

    bool IsValid() { return _fd != OSS_INVALID_FD; };
    int Read(void *buff, size_t buffSize, ssize_t &readBytes);
    int Write(const char *buff, size_t buffSize);
    offsetType GetCurrentOffset() const { return oss_lseek(_fd, 0, SEEK_CUR); };
    void SeekToOffset(offsetType &offset)
    {
        if (offset != ((oss_off_t)-1)) {
            oss_lseek(_fd, offset, SEEK_SET);
        }
    };

    void SeekToEnd() { oss_lseek(_fd, 0, SEEK_END); };
    int GetSize(offsetType &fileSize);
    int GetHandle() const { return _fd; }

protected:
    void SetFileDescriptor(int fd) { _fd = fd; };

private:
    int _fd{OSS_INVALID_FD};
    bool _stdout{false};
};
}  // namespace EMDB

#endif  // EMERALDDB_FILE_OPERATOR_H