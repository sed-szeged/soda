project(cluster_dump)

include_directories(${cluster_dump_SOURCE_DIR}
                    ${cluster_dump_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${cluster_dump_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB cluster_dump_headers ./*.h)

aux_source_directory(${cluster_dump_SOURCE_DIR} cluster_dump_src)

add_executable(cluster-dump ${cluster_dump_headers} ${cluster_dump_src})
target_link_libraries(cluster-dump SoDAEngine SoDA ${Boost_LIBRARIES} dl)