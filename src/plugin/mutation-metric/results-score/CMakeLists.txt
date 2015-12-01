project(results_score_metric_plugin)

include_directories(${results_score_metric_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${results_score_metric_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${results_score_metric_plugin_SOURCE_DIR} results_score_metric_plugin_src)

add_library(results_score SHARED ${headers} ${results_score_metric_plugin_src})
target_link_libraries(results_score SoDAEngine SoDA)
