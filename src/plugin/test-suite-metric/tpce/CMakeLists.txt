project(tpce_test_suite_metric_plugin)

include_directories(${tpce_test_suite_metric_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${tpce_test_suite_metric_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${tpce_test_suite_metric_plugin_SOURCE_DIR} tpce_test_suite_metric_plugin_src)

add_library(tpce_test_suite_metric_plugin SHARED ${headers} ${tpce_test_suite_metric_plugin_src})
target_link_libraries(tpce_test_suite_metric_plugin SoDAEngine SoDA ${Boost_LIBRARIES})
