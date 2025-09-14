# BUILD_DEFINITION
# COMPILATION_SETTINGS
# SYSTEM_PATHS
# OTHER_TOOLCHAIN_FILES



##################################################=-> BUILD_DEFINITION <-=##################################################



# Set the compiler (must be before the project function)
set(CMAKE_SYSTEM_NAME Generic)



##################################################=-> COMPILATION_SETTINGS <-=##################################################



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



##################################################=-> SYSTEM_PATHS <-=##################################################


# path to main working directory
set(PROJECT_PATH   "/home/Cyber_Wolf/Coding/1_projects/templates/c++/1_full")



##################################################=-> OTHER_TOOLCHAIN_FILES <-=##################################################



# for Conan to work
include("${PROJECT_PATH}/build/build/conan_toolchain.cmake")
