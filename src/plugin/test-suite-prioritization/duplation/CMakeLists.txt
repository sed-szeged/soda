project(duplation_prioritization_plugin)

include_directories(${duplation_prioritization_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${duplation_prioritization_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${duplation_prioritization_plugin_SOURCE_DIR} duplation_prioritization_src)

add_library(duplation_prioritization SHARED ${headers} ${duplation_prioritization_src})
target_link_libraries(duplation_prioritization SoDAEngine SoDA)
