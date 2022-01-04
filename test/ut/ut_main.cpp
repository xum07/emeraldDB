#include <gtest/gtest.h>
#include <stdlib.h>
#include "pd/Log.h"

const std::string logDir = "./log_test/";
const std::string programName = "pd_ut";

class UtEnvironment : public testing::Environment {
public:
    virtual void SetUp();
    virtual void TearDown();
};

void UtEnvironment::SetUp()
{
    EMDB::Log::Init(programName.data(), logDir.data());
    EMDB_LOG(I) << "log start record for ut tests";
}

void UtEnvironment::TearDown()
{
    EXPECT_EQ(access(logDir.data(), 0), 0);
    EXPECT_EQ(access((logDir + programName + ".INFO").data(), 0), 0);

    // clear tmp file
    EMDB::Log::Destory();
    EXPECT_EQ(system(("rm -rf " + logDir).data()), 0);
}

int main(int argc, char* argv[])
{
    ::testing::AddGlobalTestEnvironment(new UtEnvironment);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}