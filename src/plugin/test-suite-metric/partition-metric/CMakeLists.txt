project(partition_metric_test_suite_metric_plugin)

include_directories(${partition_metric_test_suite_metric_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${partition_metric_test_suite_metric_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${partition_metric_test_suite_metric_plugin_SOURCE_DIR} partition_metric_test_suite_metric_plugin_src)

add_library(partition_metric_test_suite_metric_plugin SHARED ${headers} ${partition_metric_test_suite_metric_plugin_src})
target_link_libraries(partition_metric_test_suite_metric_plugin SoDAEngine SoDA ${Boost_LIBRARIES})