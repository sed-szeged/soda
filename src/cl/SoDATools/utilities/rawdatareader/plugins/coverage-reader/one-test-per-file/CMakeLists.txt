project(OneTestPerFileCoverageReaderPlugin)

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${OneTestPerFileCoverageReaderPlugin_SOURCE_DIR} one_test_per_file_coverage_reader_src)

add_library(one_test_per_file_coverage_reader SHARED ${coverage_reader_plugin_header} ${headers} ${one_test_per_file_coverage_reader_src})
