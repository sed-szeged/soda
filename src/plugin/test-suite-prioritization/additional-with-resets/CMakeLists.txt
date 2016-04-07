project(additional_with_resets_plugin)

include_directories(${additional_with_resets_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${additional_with_resets_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${additional_with_resets_plugin_SOURCE_DIR} additional_with_resets_prioritization_src)

add_library(additional_with_resets_plugin SHARED ${headers} ${additional_with_resets_prioritization_src})
target_link_libraries(additional_with_resets_plugin SoDAEngine SoDA)
