project(fault_detection_metric_plugin)

include_directories(${fault_detection_metric_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${fault_detection_metric_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${fault_detection_metric_plugin_SOURCE_DIR} fault_detection_metric_plugin_src)

add_library(fault_detection SHARED ${headers} ${fault_detection_metric_plugin_src})
target_link_libraries(fault_detection SoDAEngine SoDA)
