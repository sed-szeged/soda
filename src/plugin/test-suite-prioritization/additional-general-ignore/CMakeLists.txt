project(additional_general_ignore_prioritization_plugin)

include_directories(${additional_general_ignore_prioritization_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${additional_general_ignore_prioritization_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${additional_general_ignore_prioritization_plugin_SOURCE_DIR} additional_general_ignore_prioritization_src)

add_library(additional_general_ignore_prioritization SHARED ${headers} ${additional_general_ignore_prioritization_src})
target_link_libraries(additional_general_ignore_prioritization SoDAEngine SoDA)
