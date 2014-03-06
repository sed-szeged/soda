project(selections)

include_directories(${selections_SOURCE_DIR}
                    ${selections_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${selections_SOURCE_DIR}/../../../lib/SoDAPlugin/inc)

file(GLOB selections_headers ./*.h)
file(GLOB_RECURSE plugin_headers ./../../../lib/SoDAPlugin/inc/*.h)

aux_source_directory(${selections_SOURCE_DIR} selections_src)

add_executable(selections ${plugin_headers} ${selections_headers} ${selections_src})
target_link_libraries(selections SoDA ${Boost_LIBRARIES} dl)

add_subdirectory(plugins)
