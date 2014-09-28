project(one_test_per_file_coverage_reader_plugin)

include_directories(${one_test_per_file_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${one_test_per_file_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${one_test_per_file_coverage_reader_plugin_SOURCE_DIR} one_test_per_file_coverage_reader_src)

add_library(one_test_per_file_coverage_reader SHARED ${headers} ${one_test_per_file_coverage_reader_src})
target_link_libraries(one_test_per_file_coverage_reader SoDAEngine SoDA ${Boost_LIBRARIES})
