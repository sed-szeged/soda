project(instrument_server)

include_directories(${instrument_server_SOURCE_DIR}
                    ${instrument_server_SOURCE_DIR}/../../../lib/SoDA/inc)

file(GLOB instrument_server_headers ./*.h)

aux_source_directory(${instrument_server_SOURCE_DIR} instrument_server_src)

add_executable(instrument-server ${instrument_server_headers} ${instrument_server_src})
target_link_libraries(instrument-server SoDA ${Boost_LIBRARIES} ${LINK_LIBRARIES})
