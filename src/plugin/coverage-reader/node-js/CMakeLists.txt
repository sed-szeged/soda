project(node_js_coverage_reader_plugin)

include_directories(${node_js_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${node_js_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${node_js_coverage_reader_plugin_SOURCE_DIR} node_js_coverage_reader_src)

add_library(node_js_coverage_reader SHARED ${headers} ${node_js_coverage_reader_src})
target_link_libraries(node_js_coverage_reader SoDAEngine SoDA ${Boost_LIBRARIES})
