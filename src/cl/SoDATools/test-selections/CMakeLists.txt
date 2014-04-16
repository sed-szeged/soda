project(test_selections)

include_directories(${test_selections_SOURCE_DIR}
                    ${test_selections_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${test_selections_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB test_selections_headers ./*.h)

aux_source_directory(${test_selections_SOURCE_DIR} test_selections_src)

add_executable(test-selections ${test_selections_headers} ${test_selections_src})
target_link_libraries(test-selections SoDAEngine SoDA ${Boost_LIBRARIES} dl)
