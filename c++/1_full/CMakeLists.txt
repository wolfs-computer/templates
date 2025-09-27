# CMakeLists.txt version: 2025.09.27

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
include("./cmake/glob_files.cmake")



##################################################=-> _PROJECT_ <-=##################################################



project(
    test_libs
    LANGUAGES CXX
    VERSION 0.0.1
)



#####=-> project paths <-=#####
# PROJECT_*_PATH

set(PROJECT_BUILD_PATH    "${PROJECT_PATH}/build") # build directory
set(PROJECT_SRC_PATH      "${PROJECT_PATH}/src") # source files
set(PROJECT_LIB_PATH      "${PROJECT_PATH}/lib") # libraries (each one in separate directory)
set(PROJECT_INC_PATH      "${PROJECT_PATH}/include") # include files
set(PROJECT_DOCS_PATH     "${PROJECT_PATH}/docs") # documentation
set(PROJECT_TESTS_PATH    "${PROJECT_PATH}/tests")

set(PROJECT_FILE_TYPES    "*.c;*.cpp;*.cc;*.cxx;*.S;*.s;*.sx;*.asm") # file types for source files
# headers are managed by cmake include_directories

set(PROJECT_SRC_FILES) # source files from src/ directory
set(PROJECT_LIB_SRC_FILES) # source files from libraries in /lib
set(PROJECT_LIB_INC_PATH) # include files from libraries in /lib



#####=-> find source files in src/ <-=#####

glob_files("${PROJECT_SRC_PATH}" "${PROJECT_FILE_TYPES}" PROJECT_SRC_FILES)



#####=-> find source files in src/ subdirs <-=#####

# get all files recursively from PROJECT_SRC_PATH
file(GLOB_RECURSE subdirs LIST_DIRECTORIES true "${PROJECT_SRC_PATH}/*")
# flag to check if empty or not
set(is_empty 1)

foreach(subdir ${subdirs})
    # append source files from sub dir
    glob_files("${subdir}" "${PROJECT_FILE_TYPES}" src_sub)
    list(APPEND PROJECT_SRC_FILES "${src_sub}")

    # print message about every subdirectory
    if(IS_DIRECTORY ${subdir})
        # first message before subdirectory listing
        if (is_empty EQUAL 1)
            cmessage(INFO "Subdirectories in src:")
            set(is_empty 0)
        endif()
        # print subdirectory name and it's path
        get_filename_component(subdir_name "${subdir}" NAME)
        message("\t\"${subdir_name}\" -- ${subdir}")
    endif()
endforeach()

# if no subdirectories
if (is_empty EQUAL 1)
    cmessage(INFO "No subdirectories in src/.")
endif()



#####=-> include header files for src/ from include/ <-=#####

if(EXISTS "${PROJECT_INC_PATH}")
    include_directories(${PROJECT_INC_PATH})
    cmessage(OK "Found include directory.")
else()
    cmessage(INFO "No include directory.")
endif()



#####=-> include header files and add source files from lib/ <-=#####
# WARN: supports only libraries in form of
# dir with source files and their headers
# TODO: support any library:
# all in one dir (supported)
# multiply nested dirs (supported)
# with CMakeLists.txt


file(GLOB libraries "${PROJECT_LIB_PATH}/*")
# flag to check if empty or not
set(is_empty 1)

foreach(lib_dir ${libraries})

    if(IS_DIRECTORY ${lib_dir})

        # find source files for library
        glob_files("${lib_dir}" "${PROJECT_FILE_TYPES}" lib_src)
        list(APPEND PROJECT_LIB_SRC_FILES "${lib_src}")

        # first message before libraries listing
        if (is_empty EQUAL 1)
            cmessage(INFO "Libraries in lib/:")
            set(is_empty 0)
        endif()

        # add header files from library in one place
        list(APPEND PROJECT_LIB_INC_PATH  "${lib_dir}")

        # print library name (directory name) if not empty
        list(LENGTH lib_src file_count)
        if(NOT file_count EQUAL 0)
            get_filename_component(lib_name "${lib_dir}" NAME)
            message("\t\"${lib_name}\" -- ${lib_dir}")
        endif()
    endif()

endforeach()

# include header files from all libraries from /lib
include_directories(${PROJECT_LIB_INC_PATH})

# if no libraries
if (is_empty EQUAL 1)
    cmessage(INFO "No libraries in lib/.")
endif()



##################################################=-> _TARGETS_ <-=##################################################



#####=-> create main target <-=#####

add_executable(${PROJECT_NAME} ${PROJECT_SRC_FILES} ${PROJECT_LIB_SRC_FILES})



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

# system
# NOTE: https://cmake.org/cmake/help/latest/command/cmake_host_system_information.html
cmake_host_system_information(RESULT host_os_info QUERY OS_NAME OS_RELEASE OS_VERSION PROCESSOR_NAME)
list(GET host_os_info 0 PROJECT_OS_NAME)
list(GET host_os_info 1 PROJECT_OS_RELEASE)
list(GET host_os_info 2 PROJECT_OS_VERSION)
list(GET host_os_info 3 PROJECT_OS_PROCESSOR_NAME)




#####=-> Create ProjectConfig.h in src/ using ProjectConfig.h.in from project root <-=#####

configure_file(${PROJECT_PATH}/ProjectConfig.h.in ${PROJECT_INC_PATH}/ProjectConfig.h @ONLY)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "${PROJECT_PATH}"
)



##################################################=-> _DOCS_ <-=##################################################
# cmake --build . -j 12 --target=docs



# TODO: test it
if(EXISTS "${PROJECT_DOCS_PATH}")
    add_subdirectory(${PROJECT_DOCS_PATH})
    cmessage(OK "Docs can be generated!")
else()
    cmessage(INFO "No docs!")
endif()



##################################################=-> _TESTS_ <-=##################################################
# cmake --build . -j 12; GTEST_COLOR=1 ctest --test-dir tests



# TODO: test it
if(EXISTS "${PROJECT_TESTS_PATH}")
    add_subdirectory(${PROJECT_TESTS_PATH})
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
    COMMAND /bin/bash -c "valgrind ${PROJECT_BUILD_PATH}/${PROJECT_NAME} --leak-check=full"
    COMMAND /bin/bash -c "rm -f ${PROJECT_BUILD_PATH}/${PROJECT_NAME}"
)


# To use it:
# cmake --build . --target=run_valgrind
add_custom_target(
    run
    DEPENDS ${PROJECT_NAME}
    COMMAND /bin/bash -c "${PROJECT_BUILD_PATH}/${PROJECT_NAME}"
    COMMAND /bin/bash -c "rm -f ${PROJECT_BUILD_PATH}/${PROJECT_NAME}"
)


# NOTE: Most used commands:
# conan install .. --output-folder=build --build=missing
# cmake .. -DCMAKE_BUILD_TYPE=Release
# cmake --build . --target=run
