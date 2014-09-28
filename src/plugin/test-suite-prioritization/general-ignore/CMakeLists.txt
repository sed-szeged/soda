project(general_ignore_prioritization_plugin)

include_directories(${general_ignore_prioritization_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${general_ignore_prioritization_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${general_ignore_prioritization_plugin_SOURCE_DIR} general_ignore_prioritization_src)

add_library(general_ignore_prioritization SHARED ${headers} ${general_ignore_prioritization_src})
target_link_libraries(general_ignore_prioritization SoDAEngine SoDA)
