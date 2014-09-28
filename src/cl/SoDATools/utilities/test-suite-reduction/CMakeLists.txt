project(test_suite_reduction)

include_directories(${test_suite_reduction_SOURCE_DIR}
                    ${test_suite_reduction_SOURCE_DIR}/../../../../lib/SoDA/inc
                    ${test_suite_reduction_SOURCE_DIR}/../../../../lib/SoDAEngine/inc)

file(GLOB reduction_headers ./*.h)

aux_source_directory(${test_suite_reduction_SOURCE_DIR} reduction_src)

if (UNIX)
	set(LINK_LIBRARIES dl)
endif()

add_executable(test-suite-reduction ${reduction_headers} ${reduction_src})
target_link_libraries(test-suite-reduction SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_LIBRARIES})
