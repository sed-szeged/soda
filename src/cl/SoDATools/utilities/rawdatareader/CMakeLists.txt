project(rawDataReader)

include_directories(${rawDataReader_SOURCE_DIR}
                    ${rawDataReader_SOURCE_DIR}/../../../../lib/SoDA/inc
                    ${rawDataReader_SOURCE_DIR}/../../../../lib/SoDAPlugin/inc)

file(GLOB rawDataReader_headers ./*.h)
file(GLOB_RECURSE plugin_headers ./../../../../lib/SoDAPlugin/inc/*.h)

aux_source_directory(${rawDataReader_SOURCE_DIR} rawDataReader_src)

add_executable(rawDataReader ${plugin_headers} ${rawDataReader_headers} ${rawDataReader_src})
target_link_libraries(rawDataReader SoDA ${Boost_LIBRARIES} dl)

add_subdirectory(plugins)
