project(SoDATest)

include_directories(
    ${SoDATest_SOURCE_DIR}/../../lib/SoDA/inc
    ${SoDATest_SOURCE_DIR}/../../lib/SoDAEngine/inc
    ${RAPIDJSON_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

add_definitions(-std=c++11)

aux_source_directory(${SoDATest_SOURCE_DIR}/SoDA soda_src)
aux_source_directory(${SoDATest_SOURCE_DIR}/plugin plugin_src)

ExternalProject_Get_Property(googletest binary_dir)
if (WIN32)
    set(SUFFIX ".lib")
else()
    set(SUFFIX ".a")
endif()

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
    ${LINK_DL}
    ${Boost_LIBRARIES})

# Copy sample directory to the build directory
add_custom_command(TARGET SoDATest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/sample $<TARGET_FILE_DIR:SoDATest>/sample)
