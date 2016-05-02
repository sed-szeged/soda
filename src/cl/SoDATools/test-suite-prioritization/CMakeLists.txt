project(test_suite_prioritization)

include_directories(${test_suite_prioritization_SOURCE_DIR}
                    ${test_suite_prioritization_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${test_suite_prioritization_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB test_suite_prioritization_headers ./*.h)

aux_source_directory(${test_suite_prioritization_SOURCE_DIR} test_suite_prioritization_src)

add_executable(test-suite-prioritization ${test_suite_prioritization_headers} ${test_suite_prioritization_src})
target_link_libraries(test-suite-prioritization SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS test-suite-prioritization RUNTIME DESTINATION bin)
