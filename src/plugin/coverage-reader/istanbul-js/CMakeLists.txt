project(istanbul_js_coverage_reader_plugin)

include_directories(${istanbul_js_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${istanbul_js_coverage_reader_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${istanbul_js_coverage_reader_plugin_SOURCE_DIR} istanbul_js_coverage_reader_src)

add_library(istanbul_js_coverage_reader SHARED ${headers} ${istanbul_js_coverage_reader_src})
target_link_libraries(istanbul_js_coverage_reader SoDAEngine SoDA ${Boost_LIBRARIES})
