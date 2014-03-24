project(GcovCoverageReaderPlugin)

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${GcovCoverageReaderPlugin_SOURCE_DIR} gcov_coverage_reader_src)

add_library(gcov_coverage_reader SHARED ${coverage_reader_plugin_header} ${headers} ${gcov_coverage_reader_src})
target_link_libraries(gcov_coverage_reader SoDA)
