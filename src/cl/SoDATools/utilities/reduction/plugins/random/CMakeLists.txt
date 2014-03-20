project(RandomReductionPlugin)

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${RandomReductionPlugin_SOURCE_DIR} random_reduction_src)

add_library(random_reduction SHARED  ${reduction_plugin_header} ${headers} ${random_reduction_src})
target_link_libraries(random_reduction SoDA)
