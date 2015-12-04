project(mutation_prioritization)

include_directories(${mutation_prioritization_SOURCE_DIR}
                    ${mutation_prioritization_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${mutation_prioritization_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB mutation_prioritization_headers ./*.h)

aux_source_directory(${mutation_prioritization_SOURCE_DIR} mutation_prioritization_src)

add_executable(mutation-prioritization ${mutation_prioritization_headers} ${mutation_prioritization_src})
target_link_libraries(mutation-prioritization SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS mutation-prioritization RUNTIME DESTINATION bin)
