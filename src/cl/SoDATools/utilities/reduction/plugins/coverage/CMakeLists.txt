project(CoverageReductionPlugin)

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${CoverageReductionPlugin_SOURCE_DIR} coverage_reduction_src)

add_library(coverage_reduction SHARED  ${reduction_plugin_header} ${headers} ${coverage_reduction_src})
target_link_libraries(coverage_reduction SoDA)
