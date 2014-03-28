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

option(test "Build tests" OFF)
option(coverage "Build with coverage analysis support" OFF)

add_definitions(-Wall)

add_definitions(-DSODA_PLUGIN_DIR="${SoDA_BINARY_DIR}/plugin")

if(test)
    if(coverage)
        set(CMAKE_BUILD_TYPE, Debug)
        if(CMAKE_COMPILER_IS_GNUCC)
            set(CMAKE_C_FLAGS "-g -O0 -coverage")
            set(CMAKE_CXX_FLAGS "-g -O0 -coverage")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage")
        endif()

        include(${CMAKE_SOURCE_DIR}/cmake/lcov.cmake)
    endif(coverage)
endif(test)

add_subdirectory(lib/SoDA)
add_subdirectory(lib/SoDAEngine)
add_subdirectory(plugin)
add_subdirectory(cl)

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
