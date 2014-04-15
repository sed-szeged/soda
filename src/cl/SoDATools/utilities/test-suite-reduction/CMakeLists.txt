project(test_suite_reduction)

include_directories(${test_suite_reduction_SOURCE_DIR}
                    ${test_suite_reduction_SOURCE_DIR}/../../../../lib/SoDA/inc
                    ${test_suite_reduction_SOURCE_DIR}/../../../../lib/SoDAEngine/inc)

file(GLOB reduction_headers ./*.h)

aux_source_directory(${test_suite_reduction_SOURCE_DIR} reduction_src)

add_executable(test_suite_reduction ${reduction_headers} ${reduction_src})
target_link_libraries(test_suite_reduction SoDAEngine SoDA ${Boost_LIBRARIES} dl)
