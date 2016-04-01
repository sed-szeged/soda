project(raptor_prioritization_plugin)

include_directories(${raptor_prioritization_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${raptor_prioritization_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${raptor_prioritization_plugin_SOURCE_DIR} raptor_prioritization_src)

add_library(raptor_prioritization SHARED ${headers} ${raptor_prioritization_src})
target_link_libraries(raptor_prioritization SoDAEngine SoDA)
