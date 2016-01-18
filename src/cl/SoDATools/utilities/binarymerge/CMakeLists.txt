project(binaryMerge)

include_directories(${binaryMerge_SOURCE_DIR}
                    ${binaryMerge_SOURCE_DIR}/../../../../lib/SoDA/inc
                    ${binaryMerge_SOURCE_DIR}/../../../../lib/SoDAEngine/inc)

file(GLOB binaryMerge_headers ./*.h)

aux_source_directory(${binaryMerge_SOURCE_DIR} binaryMerge_src)

add_executable(binaryMerge ${binaryMerge_headers} ${binaryMerge_src})
target_link_libraries(binaryMerge SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS binaryMerge RUNTIME DESTINATION bin)
