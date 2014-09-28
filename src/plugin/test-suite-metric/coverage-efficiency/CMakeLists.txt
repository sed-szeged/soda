project(coverage_efficiency_test_suite_metric_plugin)

include_directories(${coverage_efficiency_test_suite_metric_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${coverage_efficiency_test_suite_metric_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${coverage_efficiency_test_suite_metric_plugin_SOURCE_DIR} coverage_efficiency_test_suite_metric_plugin_src)

add_library(coverage_efficiency_test_suite_metric_plugin SHARED ${headers} ${coverage_efficiency_test_suite_metric_plugin_src})
target_link_libraries(coverage_efficiency_test_suite_metric_plugin SoDAEngine SoDA ${Boost_LIBRARIES})
