project(coverage)

include_directories(${coverage_SOURCE_DIR}
                    ${coverage_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${coverage_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB coverage_headers ./*.h)

aux_source_directory(${coverage_SOURCE_DIR} coverage_src)

add_executable(coverage ${coverage_headers} ${coverage_src})
target_link_libraries(coverage SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS coverage RUNTIME DESTINATION bin)
