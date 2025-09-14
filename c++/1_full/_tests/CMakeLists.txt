


#####=-> Add GoogleTest <-=#####

include(FetchContent)
FetchContent_Declare(
    googletest
    # GIT_REPOSITORY https://github.com/google/googletest.git
    # GIT_TAG        v1.14.0
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)



#####=-> Enable testing <-=#####

enable_testing()
# include (CTest)
# include(Dart)
# set(MEMORYCHECK_COMMAND valgrind)



#####=-> Create test binary <-=#####

# find test source files

file(GLOB TEST_FILES "${TESTS_PATH}/*.cpp")

# foreach(file ${TEST_FILES})
#     message("\t${file}")
# endforeach()


add_executable(
    unit_tests
    # ${TESTS_PATH}/test_test.cpp
    ${TEST_FILES}
    # ${TESTS_PATH}/other_tests.cpp
)



#####=-> Link against GTest and C code <-=#####

# Add your source files
add_library(SrcFiles ${SRC_FILES})


target_link_libraries(
    unit_tests
    PRIVATE GTest::gtest_main
    SrcFiles
    # PRIVATE GTest::gtest_main
)
# link_libraries(-fsanitize=leak,address,undefined)


#####=-> Add the test to CTest <-=#####

include(GoogleTest)
gtest_discover_tests(unit_tests)

