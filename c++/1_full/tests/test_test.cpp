#include <gtest/gtest.h>

#include "ProjectConfig.h"



// Test if GTest work properly
TEST(Project, Version) {
    EXPECT_EQ(PROJECT_VERSION, "0.0.1");
}
