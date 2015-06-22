project(clustering_metrics_test_suite_metric_plugin)

include_directories(${clustering_metrics_test_suite_metric_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${clustering_metrics_test_suite_metric_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${clustering_metrics_test_suite_metric_plugin_SOURCE_DIR} clustering_metrics_test_suite_metric_plugin_src)

add_library(clustering_metrics_test_suite_metric_plugin SHARED ${headers} ${clustering_metrics_test_suite_metric_plugin_src})
target_link_libraries(clustering_metrics_test_suite_metric_plugin SoDAEngine SoDA ${Boost_LIBRARIES})