project(partition_with_resets_plugin)

include_directories(${partition_with_resets_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${partition_with_resets_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${partition_with_resets_plugin_SOURCE_DIR} partition_with_resets_prioritization_src)

add_library(partition_with_resets_plugin SHARED ${headers} ${partition_with_resets_prioritization_src})
target_link_libraries(partition_with_resets_plugin SoDAEngine SoDA)
