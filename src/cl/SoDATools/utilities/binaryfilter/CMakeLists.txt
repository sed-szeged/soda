project(binaryFilter)

include_directories(${binaryFilter_SOURCE_DIR}
                    ${binaryFilter_SOURCE_DIR}/../../../../lib/SoDA/inc
                    ${binaryFilter_SOURCE_DIR}/../../../../lib/SoDAEngine/inc)

file(GLOB binaryFilter_headers ./*.h)

aux_source_directory(${binaryFilter_SOURCE_DIR} binaryFilter_src)

add_executable(binaryFilter ${binaryFilter_headers} ${binaryFilter_src})
target_link_libraries(binaryFilter SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS binaryFilter RUNTIME DESTINATION bin)
