project(clustering)

include_directories(${clustering_SOURCE_DIR}
                    ${clustering_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${clustering_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB clustering_headers ./*.h)

aux_source_directory(${clustering_SOURCE_DIR} clustering_src)

add_executable(clustering ${clustering_headers} ${clustering_src})
target_link_libraries(clustering SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS clustering RUNTIME DESTINATION bin)
