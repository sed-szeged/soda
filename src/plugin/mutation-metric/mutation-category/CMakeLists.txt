project(mutation_category_metric_plugin)

include_directories(${mutation_category_metric_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${mutation_category_metric_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${mutation_category_metric_plugin_SOURCE_DIR} mutation_category_metric_plugin_src)

add_library(mutation_category SHARED ${headers} ${mutation_category_metric_plugin_src})
target_link_libraries(mutation_category SoDAEngine SoDA)
