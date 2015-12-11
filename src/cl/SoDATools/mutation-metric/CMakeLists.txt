project(mutation_metric)

include_directories(${mutation_metric_SOURCE_DIR}
                    ${mutation_metric_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${mutation_metric_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB mutation_metric_headers ./*.h)

aux_source_directory(${mutation_metric_SOURCE_DIR} mutation_metric_src)

add_executable(mutation-metric ${mutation_metric_headers} ${mutation_metric_src})
target_link_libraries(mutation-metric SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS mutation-metric RUNTIME DESTINATION bin)
