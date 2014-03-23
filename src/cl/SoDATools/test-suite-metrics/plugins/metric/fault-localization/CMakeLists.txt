project(fault_localization_metric_plugin)

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${fault_localization_metric_plugin_SOURCE_DIR} fault_localization_metric_plugin_src)

add_library(fault_localization SHARED ${metric_plugin_header} ${headers} ${fault_localization_metric_plugin_src})
target_link_libraries(fault_localization SoDA)
