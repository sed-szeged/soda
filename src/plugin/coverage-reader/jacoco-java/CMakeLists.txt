project(jacoco_java_coverage_reader_plugin)

include_directories(${jacoco_java_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${jacoco_java_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${jacoco_java_coverage_reader_plugin_SOURCE_DIR} jacoco_java_coverage_reader_src)

add_library(jacoco_java_coverage_reader SHARED ${headers} ${jacoco_java_coverage_reader_src})
target_link_libraries(jacoco_java_coverage_reader SoDAEngine SoDA ${Boost_LIBRARIES})
