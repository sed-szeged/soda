project(binaryDump)

include_directories(${binaryDump_SOURCE_DIR}
                    ${binaryDump_SOURCE_DIR}/../../../../lib/SoDA/inc
                    ${binaryDump_SOURCE_DIR}/../../../../lib/SoDAEngine/inc)

aux_source_directory(${binaryDump_SOURCE_DIR} binaryDump_src)

add_executable(binaryDump ${binaryDump_src})
target_link_libraries(binaryDump SoDA ${Boost_LIBRARIES})
install(TARGETS binaryDump RUNTIME DESTINATION bin)
