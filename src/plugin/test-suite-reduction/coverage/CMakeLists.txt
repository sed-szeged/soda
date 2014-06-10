project(coverage_reduction_plugin)

include_directories(${coverage_reduction_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${coverage_reduction_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${coverage_reduction_plugin_SOURCE_DIR} coverage_reduction_src)

add_library(coverage_reduction SHARED ${headers} ${coverage_reduction_src})
target_link_libraries(coverage_reduction SoDAEngine SoDA)
