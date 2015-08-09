project(test_suite_metrics)

include_directories(${test_suite_metrics_SOURCE_DIR}
                    ${test_suite_metrics_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${test_suite_metrics_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB test_suite_metrics_headers ./*.h)

aux_source_directory(${test_suite_metrics_SOURCE_DIR} test_suite_metrics_src)

add_executable(test-suite-metrics ${test_suite_metrics_headers} ${test_suite_metrics_src})
target_link_libraries(test-suite-metrics SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS test-suite-metrics RUNTIME DESTINATION bin)
