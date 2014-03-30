project(fault_localization_metric_plugin)

include_directories(${fault_localization_metric_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${fault_localization_metric_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${fault_localization_metric_plugin_SOURCE_DIR} fault_localization_metric_plugin_src)

add_library(fault_localization SHARED ${headers} ${fault_localization_metric_plugin_src})
target_link_libraries(fault_localization SoDAEngine SoDA)
