project(mutation_score)

include_directories(${mutation_score_SOURCE_DIR}
                    ${mutation_score_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${mutation_score_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB mutation_score_headers ./*.h)

aux_source_directory(${mutation_score_SOURCE_DIR} mutation_score_src)

add_executable(mutation-score ${mutation_score_headers} ${mutation_score_src})
target_link_libraries(mutation-score SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS mutation-score RUNTIME DESTINATION bin)
