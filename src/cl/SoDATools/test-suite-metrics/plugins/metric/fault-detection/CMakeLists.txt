project(fault_detection_metric_plugin)

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${fault_detection_metric_plugin_SOURCE_DIR} fault_detection_metric_plugin_src)

add_library(fault_detection SHARED ${metric_plugin_header} ${headers} ${fault_detection_metric_plugin_src})
target_link_libraries(fault_detection SoDA)
