project(SoDALibTest)

include_directories(
    ${SoDALibTest_SOURCE_DIR}/../../lib/SoDA/inc
    ${SoDALibTest_SOURCE_DIR}/../../lib/SoDATools/inc
    ${Boost_INCLUDE_DIRS})

aux_source_directory(${SoDALibTest_SOURCE_DIR}/SoDA soda_src)

ExternalProject_Get_Property(googletest binary_dir)
set(SUFFIX ".a")

add_executable(SoDATest SoDATest.cpp ${soda_src} ${sodatools_src})

# Create dependency of MainTest on googletest
add_dependencies(SoDATest googletest)

target_link_libraries(
    SoDATest
    debug ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${SUFFIX}
    optimized ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${SUFFIX}
    ${CMAKE_THREAD_LIBS_INIT}
    SoDA
    ${Boost_LIBRARIES})

# Copy sample directory to the build directory
file(COPY sample/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/sample/)
