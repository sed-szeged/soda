project(test_suite_selections)

include_directories(${test_suite_selections_SOURCE_DIR}
                    ${test_suite_selections_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${test_suite_selections_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB test_suite_selections_headers ./*.h)

aux_source_directory(${test_suite_selections_SOURCE_DIR} test_suite_selections_src)

add_executable(test-suite-selections ${test_suite_selections_headers} ${test_suite_selections_src})
target_link_libraries(test-suite-selections SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS test-suite-selections RUNTIME DESTINATION bin)
