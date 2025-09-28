# _BUILD_DEFINITION_
# _COMPILATION_SETTINGS_
# _SYSTEM_PATHS_
# _OTHER_TOOLCHAIN_FILES_



##################################################=-> _BUILD_DEFINITION_ <-=##################################################



# the name of the target operating system (must be before the project function)
set(CMAKE_SYSTEM_NAME Android)



##################################################=-> _COMPILATION_SETTINGS_ <-=##################################################



#########################=-> C <-=########################

set(CMAKE_C_COMPILER           gcc)
set(CMAKE_C_STANDARD           99) # standard to use
set(CMAKE_C_STANDARD_REQUIRED  ON) # enforce the standard

# debug with gdb
# set(CDEBUG    "${CMAKE_C_FLAGS} -gstabs -g -ggdb")

# add more info to executable
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -g")

# other
# set(CWARN     "${CMAKE_C_FLAGS} -Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
# set(CTUNING   "${CMAKE_C_FLAGS} -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
# set(COPT      "${CMAKE_C_FLAGS} -Os -lm -lprintf_flt")



#########################=-> CXX <-=########################

set(CMAKE_CXX_COMPILER           g++)
set(CMAKE_CXX_STANDARD           17) # standard to use
set(CMAKE_CXX_STANDARD_REQUIRED  ON) # enforce the standard

# add more info to executable
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -g")



#########################=-> ccache <-=########################

# use ccache if found
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
   set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()



##################################################=-> _SYSTEM_PATHS_ <-=##################################################


# path to main working directory
set(PROJECT_PATH   "${CMAKE_SOURCE_DIR}")
# set(PROJECTS_PATH "/data/data/com.termux/files/home/2_projects")



##################################################=-> _OTHER_TOOLCHAIN_FILES_ <-=##################################################



# for Conan to work (needs to be executed 2 times)
if(EXISTS "${PROJECT_PATH}/build/CMakeCache.txt")
    include("${PROJECT_PATH}/build/build/conan_toolchain.cmake")
endif()
