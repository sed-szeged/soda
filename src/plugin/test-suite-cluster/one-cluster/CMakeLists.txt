project(one_cluster_plugin)

include_directories(${one_cluster_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${one_cluster_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${one_cluster_plugin_SOURCE_DIR} one_cluster_plugin_src)

add_library(one_cluster SHARED ${headers} ${one_cluster_plugin_src})
target_link_libraries(one_cluster SoDAEngine SoDA)
