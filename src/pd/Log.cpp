#include "Log.h"
#include <string>
#include <iostream>
#include <sys/stat.h>

using namespace EMDB;

bool Log::Init(const char *programName, const char* logDir)
{
    errno = 0;
    if (access(logDir, 0) != 0) {
        if (mkdir(logDir, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH) != 0) {
            std::cout << programName << " failed to create log dir" << logDir << ", errno=" << errno << std::endl;
            return false;
        }
    }

    google::InitGoogleLogging(programName);
    google::SetLogDestination(google::INFO, logDir);
    google::InstallFailureSignalHandler();
    google::InstallFailureWriter(&Writer);
    FLAGS_alsologtostderr = 0;
    FLAGS_max_log_size = 50;
    return true;
}

void Log::Destory()
{
    google::ShutdownGoogleLogging();
}

void Log::Writer(const char *data, size_t size)
{
    EMDB_LOG(E) << data;
}