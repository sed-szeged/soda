project(test_selection_statistics)

include_directories(${test_selection_statistics_SOURCE_DIR}
                    ${test_selection_statistics_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${test_selection_statistics_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB test_selection_statistics_headers ./*.h)

aux_source_directory(${test_selection_statistics_SOURCE_DIR} test_selection_statistics_src)

add_executable(test-selection-statistics ${test_selection_statistics_headers} ${test_selection_statistics_src})
target_link_libraries(test-selection-statistics SoDA ${Boost_LIBRARIES})
install(TARGETS test-selection-statistics RUNTIME DESTINATION bin)
