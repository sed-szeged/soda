project(partition_efficiency_test_suite_metric_plugin)

include_directories(${partition_efficiency_test_suite_metric_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${partition_efficiency_test_suite_metric_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${partition_efficiency_test_suite_metric_plugin_SOURCE_DIR} partition_efficiency_test_suite_metric_plugin_src)

add_library(partition_efficiency_test_suite_metric_plugin SHARED ${headers} ${partition_efficiency_test_suite_metric_plugin_src})
target_link_libraries(partition_efficiency_test_suite_metric_plugin SoDAEngine SoDA ${Boost_LIBRARIES})
