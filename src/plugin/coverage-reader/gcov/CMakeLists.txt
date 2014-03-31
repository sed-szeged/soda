project(gcov_coverage_reader_plugin)

include_directories(${gcov_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${gcov_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${gcov_coverage_reader_plugin_SOURCE_DIR} gcov_coverage_reader_src)

add_library(gcov_coverage_reader SHARED ${headers} ${gcov_coverage_reader_src})
target_link_libraries(gcov_coverage_reader SoDAEngine SoDA ${Boost_LIBRARIES})
