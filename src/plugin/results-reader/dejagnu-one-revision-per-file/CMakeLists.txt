project(dejagnu_one_revision_per_file_plugin)

include_directories(${dejagnu_one_revision_per_file_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${dejagnu_one_revision_per_file_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${dejagnu_one_revision_per_file_plugin_SOURCE_DIR} dejagnu_one_revision_per_file_results_reader_src)

add_library(dejagnu_one_revision_per_file_plugin SHARED ${headers} ${dejagnu_one_revision_per_file_results_reader_src})
target_link_libraries(dejagnu_one_revision_per_file_plugin SoDAEngine SoDA)
