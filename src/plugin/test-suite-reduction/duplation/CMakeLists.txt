project(duplation_reduction_plugin)

include_directories(${duplation_reduction_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${duplation_reduction_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${duplation_reduction_plugin_SOURCE_DIR} duplation_reduction_src)

add_library(duplation_reduction SHARED ${headers} ${duplation_reduction_src})
target_link_libraries(duplation_reduction SoDAEngine SoDA)
