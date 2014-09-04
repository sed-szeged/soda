project(duplation_test_suite_cluster_plugin)

include_directories(${duplation_test_suite_cluster_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${duplation_test_suite_cluster_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${duplation_test_suite_cluster_plugin_SOURCE_DIR} duplation_test_suite_cluster_plugin_src)

add_library(duplation_test_suite_cluster_plugin SHARED ${headers} ${duplation_test_suite_cluster_plugin_src})
target_link_libraries(duplation_test_suite_cluster_plugin SoDAEngine SoDA ${Boost_LIBRARIES})