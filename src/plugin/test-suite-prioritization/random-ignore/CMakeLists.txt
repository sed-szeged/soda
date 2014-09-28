project(random_ignore_prioritization_plugin)

include_directories(${random_ignore_prioritization_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${random_ignore_prioritization_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${random_ignore_prioritization_plugin_SOURCE_DIR} random_ignore_prioritization_src)

add_library(random_ignore_prioritization SHARED ${headers} ${random_ignore_prioritization_src})
target_link_libraries(random_ignore_prioritization SoDAEngine SoDA)
