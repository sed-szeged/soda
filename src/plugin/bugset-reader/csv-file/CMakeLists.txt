project(csv_file_bugset_reader_plugin)

include_directories(${csv_file_bugset_reader_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${csv_file_bugset_reader_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${csv_file_bugset_reader_plugin_SOURCE_DIR} csv_file_bugset_reader_src)

add_library(csv_file_bugset_reader SHARED ${headers} ${csv_file_bugset_reader_src})
target_link_libraries(csv_file_bugset_reader SoDAEngine SoDA)
