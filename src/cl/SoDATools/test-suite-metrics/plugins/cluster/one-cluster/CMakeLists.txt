project(one_cluster_plugin)

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${one_cluster_plugin_SOURCE_DIR} one_cluster_plugin_src)

add_library(one_cluster SHARED ${cluster_plugin_header} ${headers} ${one_cluster_plugin_src})
target_link_libraries(one_cluster SoDA)
