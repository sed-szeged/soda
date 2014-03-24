project(test_suite_metrics)

include_directories(${test_suite_metrics_SOURCE_DIR}
                    ${test_suite_metrics_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${test_suite_metrics_SOURCE_DIR}/../../../lib/SoDAPlugin/inc)

file(GLOB test_suite_metrics_headers ./*.h)
file(GLOB_RECURSE plugin_headers ./../../../../lib/SoDAPlugin/inc/*.h)

aux_source_directory(${test_suite_metrics_SOURCE_DIR} test_suite_metrics_src)

add_executable(test-suite-metrics ${plugin_headers} ${test_suite_metrics_headers} ${test_suite_metrics_src})
target_link_libraries(test-suite-metrics SoDA ${Boost_LIBRARIES} dl)

add_subdirectory(plugins)
