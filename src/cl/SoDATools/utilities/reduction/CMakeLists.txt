project(reduction)

include_directories(${reduction_SOURCE_DIR}
                    ${reduction_SOURCE_DIR}/../../../../lib/SoDA/inc
                    ${reduction_SOURCE_DIR}/../../../../lib/SoDAPlugin/inc)

file(GLOB reduction_headers ./*.h)
file(GLOB_RECURSE plugin_headers ./../../../../lib/SoDAPlugin/inc/*.h)

aux_source_directory(${reduction_SOURCE_DIR} reduction_src)

add_executable(reduction ${plugin_headers} ${reduction_data_src} ${reduction_headers} ${reduction_src})
target_link_libraries(reduction SoDA ${Boost_LIBRARIES} dl)

add_subdirectory(plugins)
