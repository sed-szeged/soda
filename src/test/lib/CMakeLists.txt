project(SoDATest)

include_directories(
    ${SoDATest_SOURCE_DIR}/../../lib/SoDA/inc
    ${SoDATest_SOURCE_DIR}/../../lib/SoDAEngine/inc
    ${RAPIDJSON_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

aux_source_directory(${SoDATest_SOURCE_DIR}/SoDA soda_src)
aux_source_directory(${SoDATest_SOURCE_DIR}/plugin plugin_src)

ExternalProject_Get_Property(googletest binary_dir)
set(SUFFIX ".a")

add_executable(SoDATest SoDATest.cpp ${soda_src} ${plugin_src})

# Create dependency of MainTest on googletest
add_dependencies(SoDATest googletest)

target_link_libraries(
    SoDATest
    debug ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${SUFFIX}
    optimized ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${SUFFIX}
    ${CMAKE_THREAD_LIBS_INIT}
    SoDA
    SoDAEngine
    dl
    ${Boost_LIBRARIES})

# Copy sample directory to the build directory
file(COPY sample/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/sample/)
