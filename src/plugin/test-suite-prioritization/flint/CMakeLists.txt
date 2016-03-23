project(flint_prioritization_plugin)

include_directories(${flint_prioritization_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${flint_prioritization_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${flint_prioritization_plugin_SOURCE_DIR} flint_prioritization_src)

add_library(flint_prioritization SHARED ${headers} ${flint_prioritization_src})
target_link_libraries(flint_prioritization SoDAEngine SoDA)
