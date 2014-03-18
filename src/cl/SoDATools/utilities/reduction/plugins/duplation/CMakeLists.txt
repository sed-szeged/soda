project(DuplationReductionPlugin)

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${DuplationReductionPlugin_SOURCE_DIR} duplation_reduction_src)

add_library(duplation_reduction SHARED  ${reduction_plugin_header} ${headers} ${duplation_reduction_src})
target_link_libraries(duplation_reduction SoDA)
