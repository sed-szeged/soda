project(one_revision_per_file_changeset_reader_plugin)

include_directories(${one_revision_per_file_changeset_reader_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${one_revision_per_file_changeset_reader_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${one_revision_per_file_changeset_reader_plugin_SOURCE_DIR} one_revision_per_file_changeset_reader_src)

add_library(one_revision_per_file_changeset_reader SHARED ${headers} ${one_revision_per_file_changeset_reader_src})
target_link_libraries(one_revision_per_file_changeset_reader SoDAEngine SoDA)
