project(selections)

include_directories(${selections_SOURCE_DIR}
                    ${selections_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${selections_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB selections_headers ./*.h)

aux_source_directory(${selections_SOURCE_DIR} selections_src)

add_executable(selections ${selections_headers} ${selections_src})
target_link_libraries(selections SoDAEngine SoDA ${Boost_LIBRARIES} dl)
