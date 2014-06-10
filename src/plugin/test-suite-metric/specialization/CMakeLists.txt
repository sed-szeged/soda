project(specialization_metric_plugin)

include_directories(${specialization_metric_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${specialization_metric_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${specialization_metric_plugin_SOURCE_DIR} specialization_metric_plugin_src)

add_library(specialization SHARED ${headers} ${specialization_metric_plugin_src})
target_link_libraries(specialization SoDAEngine SoDA)
