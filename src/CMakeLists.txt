cmake_minimum_required(VERSION 2.8) 
project(SoDA)

# Set the build type if it isn't already
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system filesystem regex program_options)

find_package(Threads REQUIRED)

add_subdirectory(lib/SoDA)
add_subdirectory(lib/SoDATools)
add_subdirectory(cl)

option(test "Build tests" OFF)

if(test)
    # Enable ExternalProject CMake module
    include(ExternalProject)

    # Set default ExternalProject root directory
    set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

    # Add gtest
    ExternalProject_Add(
        googletest
        SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
        SVN_REVISION -r 671
        TIMEOUT 10
        # Force separate output paths for debug and release builds to allow easy
        # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                   -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
                   -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
                   -Dgtest_force_shared_crt=ON
        # Disable install step
        INSTALL_COMMAND ""
        # Wrap download, configure and build steps in a script to log output
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON)

    # Specify include dir
    ExternalProject_Get_Property(googletest source_dir)
    include_directories(${source_dir}/include)

    add_subdirectory(test/lib)

    enable_testing()
    add_test(
        NAME SoDATest
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/lib
        COMMAND SoDATest
    )

endif()
