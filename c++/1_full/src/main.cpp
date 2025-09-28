// standard libraries
// C
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h> // check if math.h is included
#include <X11/Xlib.h> // check if X11 is included
// C++
#include <iostream>

// library includes
#include <zlib.h>
#include <spdlog/spdlog.h>
#include "tl.h"
// #include "tl2.h"

// project include files
#include "ProjectConfig.h"
#include "test_subdir/sub.h"
#include "test_subdir/subdir2/testing_your_boundaries.h"



void check_zlib() {
    std::cout << "\nZLIB check:" << std::endl;

    char buffer_in [256] = {"Conan is a MIT-licensed, Open Source package manager for C and C++ development "
                            "for C and C++ development, allowing development teams to easily and efficiently "
                            "manage their packages and dependencies across platforms and build systems."};
    char buffer_out [256] = {0};

    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.avail_in = (uInt) strlen(buffer_in);
    defstream.next_in = (Bytef *) buffer_in;
    defstream.avail_out = (uInt) sizeof(buffer_out);
    defstream.next_out = (Bytef *) buffer_out;

    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    printf("Uncompressed size is: %lu\n", strlen(buffer_in));
    printf("Compressed size is: %lu\n", strlen(buffer_out));

    printf("ZLIB VERSION: %s\n", zlibVersion());
}


void check_spdlog() {
    std::cout << "\nspdlog check:" << std::endl;

    spdlog::info("Check!");
}



int main(void) {

    std::cout << "PROJECT_VERSION: " << PROJECT_VERSION << std::endl;
    std::cout << "PROJECT_CXX_COMPILER: : " << PROJECT_CXX_COMPILER << std::endl;
    std::cout << "PROJECT_CXX_COMPILER_VERSION: : " << PROJECT_CXX_COMPILER_VERSION << std::endl;

    std::cout << "PROJECT_OS_NAME: " << PROJECT_OS_NAME << std::endl;
    std::cout << "PROJECT_OS_VERSION: " << PROJECT_OS_VERSION << std::endl;
    std::cout << "PROJECT_OS_RELEASE: " << PROJECT_OS_RELEASE << std::endl;
    std::cout << "PROJECT_OS_PROCESSOR_NAME: " << PROJECT_OS_PROCESSOR_NAME << std::endl;

    check_subdir_1();
    check_subdir_2();

    tl::check();
    // tl2::check();

    check_zlib();

    check_spdlog();


    std::cout << "\n";


    return EXIT_SUCCESS;
}
