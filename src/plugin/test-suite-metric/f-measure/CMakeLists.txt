project(f_measure_metric_plugin)

include_directories(${f_measure_metric_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${f_measure_metric_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${f_measure_metric_plugin_SOURCE_DIR} f_measure_metric_plugin_src)

add_library(f_measure SHARED ${headers} ${f_measure_metric_plugin_src})
target_link_libraries(f_measure SoDAEngine SoDA)
