project(test_suite_score)

include_directories(${test_suite_score_SOURCE_DIR}
                    ${test_suite_score_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${test_suite_score_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB test_suite_score_headers ./*.h)

aux_source_directory(${test_suite_score_SOURCE_DIR} test_suite_score_src)

add_executable(test-suite-score ${test_suite_score_headers} ${test_suite_score_src})
target_link_libraries(test-suite-score SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS test-suite-score RUNTIME DESTINATION bin)
