project(rawDataReader)

include_directories(${rawDataReader_SOURCE_DIR}
                    ${rawDataReader_SOURCE_DIR}/../../../../lib/SoDA/inc
                    ${rawDataReader_SOURCE_DIR}/../../../../lib/SoDAEngine/inc)

file(GLOB rawDataReader_headers ./*.h)

aux_source_directory(${rawDataReader_SOURCE_DIR} rawDataReader_src)

add_executable(rawDataReader ${rawDataReader_headers} ${rawDataReader_src})
target_link_libraries(rawDataReader SoDAEngine SoDA ${Boost_LIBRARIES} dl)
