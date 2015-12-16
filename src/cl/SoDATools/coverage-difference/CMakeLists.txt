project(coverage_difference)

include_directories(${coverage_difference_SOURCE_DIR}
                    ${coverage_difference_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${coverage_difference_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB coverage_diff_headers ./*.h)

aux_source_directory(${coverage_difference_SOURCE_DIR} coverage_diff_src)

add_executable(coverage-difference ${coverage_diff_headers} ${coverage_diff_src})
target_link_libraries(coverage-difference SoDAEngine SoDA ${Boost_LIBRARIES})
install(TARGETS coverage-difference RUNTIME DESTINATION bin)
