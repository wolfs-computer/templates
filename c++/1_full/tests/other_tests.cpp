#include <gtest/gtest.h>

#include <spdlog/spdlog.h>

#include "ProjectConfig.h"
#include "tl.h"



// Test if GTest work properly
TEST(library, check_func) {
    tl::check();

    std::cout << "\nspdlog check:" << std::endl;

    spdlog::info("Check!");
}
