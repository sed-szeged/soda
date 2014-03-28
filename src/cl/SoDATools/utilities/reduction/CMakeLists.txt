project(reduction)

include_directories(${reduction_SOURCE_DIR}
                    ${reduction_SOURCE_DIR}/../../../../lib/SoDA/inc
                    ${reduction_SOURCE_DIR}/../../../../lib/SoDAEngine/inc)

file(GLOB reduction_headers ./*.h)

aux_source_directory(${reduction_SOURCE_DIR} reduction_src)

add_executable(reduction ${reduction_headers} ${reduction_src})
target_link_libraries(reduction SoDAEngine SoDA ${Boost_LIBRARIES} dl)
