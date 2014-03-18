project(AdditionalCoverageReductionPlugin)

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${AdditionalCoverageReductionPlugin_SOURCE_DIR} additional_coverage_reduction_src)

add_library(additional_coverage_reduction SHARED ${reduction_plugin_header} ${headers} ${additional_coverage_reduction_src})
target_link_libraries(additional_coverage_reduction SoDA)
