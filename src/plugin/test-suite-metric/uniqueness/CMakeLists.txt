project(uniqueness_test_suite_metric_plugin)

include_directories(${uniqueness_test_suite_metric_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${uniqueness_test_suite_metric_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${uniqueness_test_suite_metric_plugin_SOURCE_DIR} uniqueness_test_suite_metric_plugin_src)

add_library(uniqueness_test_suite_metric_plugin SHARED ${headers} ${uniqueness_test_suite_metric_plugin_src})
target_link_libraries(uniqueness_test_suite_metric_plugin SoDAEngine SoDA ${Boost_LIBRARIES})
