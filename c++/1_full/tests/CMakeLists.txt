


#####=-> Add GoogleTest <-=#####

include(FetchContent)
FetchContent_Declare(
    googletest
    # GIT_REPOSITORY https://github.com/google/googletest.git
    # GIT_TAG        v1.14.0
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP 1
)
FetchContent_MakeAvailable(googletest)



#####=-> Enable testing <-=#####

enable_testing()
# include (CTest)
# include(Dart)
# set(MEMORYCHECK_COMMAND valgrind)



#####=-> Create test binary <-=#####

# find test source files

file(GLOB TEST_FILES "${PROJECT_TESTS_PATH}/*.cpp")

# foreach(file ${TEST_FILES})
#     message("\t${file}")
# endforeach()


add_executable(
    unit_tests
    # ${PROJECT_TESTS_PATH}/test_test.cpp
    ${TEST_FILES}
    # ${PROJECT_TESTS_PATH}/other_tests.cpp
)



#####=-> Link against GTest and C code <-=#####

# Add your source files
add_library(SrcFiles ${PROJECT_SRC_FILES} ${PROJECT_LIB_SRC_FILES})


target_link_libraries(
    unit_tests
    PRIVATE GTest::gtest_main
    SrcFiles
    ${CONAN_LIBS}
    ${SYSTEM_LIBS}
)
# link_libraries(-fsanitize=leak,address,undefined)


#####=-> Add the test to CTest <-=#####

include(GoogleTest)
gtest_discover_tests(unit_tests)

