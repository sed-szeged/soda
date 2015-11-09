project(simple_instrumentation_listener_coverage_reader_plugin)

include_directories(${simple_instrumentation_listener_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${simple_instrumentation_listener_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${simple_instrumentation_listener_coverage_reader_plugin_SOURCE_DIR} simple_instrumentation_listener_coverage_reader_src)

add_library(simple_instrumentation_listener_coverage_reader SHARED ${headers} ${simple_instrumentation_listener_coverage_reader_src})
target_link_libraries(simple_instrumentation_listener_coverage_reader SoDAEngine SoDA ${Boost_LIBRARIES})
