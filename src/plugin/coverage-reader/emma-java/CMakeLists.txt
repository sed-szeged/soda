project(emma_java_coverage_reader_plugin)

include_directories(${emma_java_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${emma_java_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${emma_java_coverage_reader_plugin_SOURCE_DIR} emma_java_coverage_reader_src)

add_library(emma_java_coverage_reader SHARED ${headers} ${emma_java_coverage_reader_src})
target_link_libraries(emma_java_coverage_reader SoDAEngine SoDA ${Boost_LIBRARIES})
