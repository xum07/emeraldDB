#ifndef EMERALDDB_LOG_H
#define EMERALDDB_LOG_H

#include <glog/logging.h>

#define EMDB_LOG_I LOG(INFO)
#define EMDB_LOG_W LOG(WARNING)
#define EMDB_LOG_E LOG(ERROR)
#define EMDB_LOG_F LOG(FATAL)
#define EMDB_LOG(severity) EMDB_LOG_##severity
#define EMDB_LOG_IF(severity, condition) !(condition) ? (void)(0) : google::LogMessageVoidify() & EMDB_LOG_##severity

namespace EMDB {
enum LogSeverity : int {
    I = 0,  // info
    W,      // warning
    E,      // error
    F,      // fatal
};

const char* const LOG_DIR = "./log/";

class Log {
public:
    static bool Init(const char *programName, const char* logDir = LOG_DIR);
    static void Destory();
    static void Writer(const char *data, size_t size);
};

}  // namespace EMDB

#endif  // EMERALDDB_LOG_H