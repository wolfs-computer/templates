# _CMAKE_
# _SYSTEM_TOOLCHAIN_
# _CMAKE_INCLUDES_
# _PROJECT_
# _TARGETS_
# _CONFIG_HEADER_FILE_
# _DOCS_
# _TESTS_
# _CUSTOM_TARGETS_



##################################################=-> _CMAKE_ <-=##################################################



cmake_minimum_required(VERSION 3.11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for IDE's LSP

set(CMAKE_MAKE_PROGRAM="make -j 9")



##################################################=-> _SYSTEM_TOOLCHAIN_ <-=##################################################



# set sytem specific settings (compiler, ...)
execute_process(COMMAND bash -c "echo -n $OSTYPE" OUTPUT_VARIABLE SYSTEM_TYPE)

if("${SYSTEM_TYPE}" STREQUAL "linux-android")
    message("Building for Termux")
    set(CMAKE_TOOLCHAIN_FILE "cmake/toolchains/termux.cmake")
else()
    message("Building for Linux")
    set(CMAKE_TOOLCHAIN_FILE "cmake/toolchains/linux.cmake")
endif()



##################################################=-> _CMAKE_INCLUDES_ <-=##################################################



# pretty messages
# function cmessage()
include("./cmake/cmessage.cmake")



##################################################=-> _PROJECT_ <-=##################################################



project(
    test_libs
    LANGUAGES CXX
    VERSION 0.0.1
)



#####=-> project paths <-=#####

set(BUILD_DIR_PATH  "${PROJECT_PATH}/build")
set(SRC_PATH        "${PROJECT_PATH}/src")
set(LIB_DIR_PATH    "${PROJECT_PATH}/lib")
set(INC_PATH        "${PROJECT_PATH}/include")
set(DOCS_PATH       "${PROJECT_PATH}/docs")
set(TESTS_PATH      "${PROJECT_PATH}/tests")



#####=-> find source files in src/ <-=#####

file(GLOB SRC_FILES
    "${SRC_PATH}/*.c"
    "${SRC_PATH}/*.cpp"
    "${SRC_PATH}/*.cc"
    "${SRC_PATH}/*.cxx"
    "${SRC_PATH}/*.S"
    "${SRC_PATH}/*.s"
    "${SRC_PATH}/*.sx"
    "${SRC_PATH}/*.asm"
)



#####=-> find source files in src/ subdirs <-=#####
# TODO: test this and finish
# TODO: normal variables

# get all files recursively from SRC_PATH
file(GLOB_RECURSE SUB_DIRS LIST_DIRECTORIES true "${SRC_PATH}/*")
# flag to check if empty or not
set(is_empty 1)

foreach(subdir ${SUB_DIRS})

    # append src files from sub dir
    file(GLOB src_sub
        "${SRC_PATH}/*.c"
        "${SRC_PATH}/*.cpp"
        "${SRC_PATH}/*.cc"
        "${SRC_PATH}/*.cxx"
        "${SRC_PATH}/*.S"
        "${SRC_PATH}/*.s"
        "${SRC_PATH}/*.sx"
        "${SRC_PATH}/*.asm"
    )
    list(APPEND SRC_FILES "${src_sub}")

    # pretty message about every sub dir
    if(IS_DIRECTORY ${subdir})
        if (is_empty EQUAL 1)
            cmessage(INFO "Subdirectories in src:")
            # message("${SUB_DIRS}")
            set(is_empty 0)
        endif()

        get_filename_component(subdir_name "${subdir}" NAME)
        message("\t\"${subdir_name}\" -- ${subdir}")
    endif()

endforeach()

if (is_empty EQUAL 1)
    cmessage(INFO "No subdirectories in src/.")
endif()



#####=-> include header files for src/ from include/ <-=#####

if(EXISTS "${INC_PATH}")
    include_directories(${INC_PATH})
    cmessage(OK "Found include directory.")
else()
    cmessage(INFO "No include directory.")
endif()



#####=-> find source files in lib/ <-=#####
# TODO:
# test it
# make it prettier
# TODO: normal variables

set(LIB_SRC_FILES)
set(LIB_INC_PATH)
file(GLOB LIBRARIES "${LIB_DIR_PATH}/*")
# flag to check if empty or not
set(is_empty 1)

foreach(subdir ${LIBRARIES})

    file(GLOB lib_src
        "${LIB_PATH}/*.c"
        "${LIB_PATH}/*.cpp"
        "${LIB_PATH}/*.cc"
        "${LIB_PATH}/*.cxx"
        "${LIB_PATH}/*.S"
        "${LIB_PATH}/*.s"
        "${LIB_PATH}/*.sx"
        "${LIB_PATH}/*.asm"
    )
    list(APPEND LIB_SRC_FILES "${lib_src}")

    if(IS_DIRECTORY ${subdir})
        if (is_empty EQUAL 1)
            cmessage(INFO "Libraries in lib/:")
            set(is_empty 0)
        endif()

        list(APPEND LIB_INC_PATH  "${subdir}")

        get_filename_component(lib_name "${subdir}" NAME)
        message("\t\"${lib_name}\" -- ${subdir}")
    endif()

endforeach()

include_directories(${LIB_INC_PATH})

# if no directories
if (is_empty EQUAL 1)
    cmessage(INFO "No libraries in lib/.")
endif()



##################################################=-> _TARGETS_ <-=##################################################



#####=-> create main target <-=#####

add_executable(${PROJECT_NAME} ${SRC_FILES} ${LIB_SRC_FILES})



#####=-> specify output name/location <-=#####

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")



#####=-> link libs to main target <-=#####
# conan install .. --output-folder=build --build=missing
# cmake .. -DCMAKE_BUILD_TYPE=Release


# libraries must be available at system
set(SYSTEM_LIBS
    m
    X11
)

# libraries installed by conan
find_package(ZLIB REQUIRED)
find_package(spdlog REQUIRED)

set(CONAN_LIBS
    ZLIB::ZLIB
    spdlog::spdlog
)

# NOTE:
# PUBLIC: The library is needed by the target and its consumers.
# PRIVATE: The library is only needed by the target itself.
# INTERFACE: The library is not used by the target but is required by its consumers (e.g., for header-only libraries or specific build flags).p

# NOTE:
# find_package(Boost COMPONENTS filesystem system REQUIRED)
# target_link_libraries(MyExecutable PUBLIC Boost::filesystem Boost::system)

# link all libraries
target_link_libraries(
    ${PROJECT_NAME}
    ${CONAN_LIBS}
    ${SYSTEM_LIBS}
)



##################################################=-> _CONFIG_HEADER_FILE_ <-=##################################################



#####=-> ProjectConfig.h data <-=#####

# C compiler
get_filename_component(C_COMPILER_BASENAME "${CMAKE_C_COMPILER}" NAME)
set(PROJECT_C_COMPILER "${C_COMPILER_BASENAME}")
set(PROJECT_C_COMPILER_VERSION "${CMAKE_C_COMPILER_VERSION}")

# CXX compiler
get_filename_component(CXX_COMPILER_BASENAME "${CMAKE_CXX_COMPILER}" NAME)
set(PROJECT_CXX_COMPILER "${CXX_COMPILER_BASENAME}")
set(PROJECT_CXX_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")



#####=-> Create ProjectConfig.h in src/ using ProjectConfig.h.in from project root <-=#####

configure_file(${PROJECT_PATH}/ProjectConfig.h.in ${INC_PATH}/ProjectConfig.h @ONLY)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "${PROJECT_PATH}"
)



##################################################=-> _DOCS_ <-=##################################################
# cmake --build . -j 12 --target=docs



# TODO: test it
if(EXISTS "${DOCS_PATH}")
    add_subdirectory(${DOCS_PATH})
    cmessage(OK "Docs can be generated!")
else()
    cmessage(INFO "No docs!")
endif()



##################################################=-> _TESTS_ <-=##################################################
# cmake --build . -j 12; GTEST_COLOR=1 ctest --test-dir tests



# TODO: test it
if(EXISTS "${TESTS_PATH}")
    add_subdirectory(${TESTS_PATH})
    cmessage(OK "Tests can be used!")
else()
    cmessage(INFO "No tests!")
endif()



##################################################=-> _CUSTOM_TARGETS_ <-=##################################################
# cmake .. -DCMAKE_BUILD_TYPE=Release



#####=-> run executable (run) <-=#####

# To use it:
# cmake --build . --target=run
# first it should be just: cmake --build .
add_custom_target(
    run_valgrind
    DEPENDS ${PROJECT_NAME}
    COMMAND /bin/bash -c "valgrind ${BUILD_DIR_PATH}/${PROJECT_NAME} --leak-check=full"
    COMMAND /bin/bash -c "rm -f ${BUILD_DIR_PATH}/${PROJECT_NAME}"
)


# To use it:
# cmake --build . --target=run_valgrind
add_custom_target(
    run
    DEPENDS ${PROJECT_NAME}
    COMMAND /bin/bash -c "${BUILD_DIR_PATH}/${PROJECT_NAME}"
    COMMAND /bin/bash -c "rm -f ${BUILD_DIR_PATH}/${PROJECT_NAME}"
)
