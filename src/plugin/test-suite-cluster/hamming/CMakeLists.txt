project(hamming_test_suite_cluster_plugin)

include_directories(${hamming_test_suite_cluster_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${hamming_test_suite_cluster_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${hamming_test_suite_cluster_plugin_SOURCE_DIR} hamming_test_suite_cluster_plugin_src)

add_library(hamming_test_suite_cluster_plugin SHARED ${headers} ${hamming_test_suite_cluster_plugin_src})
target_link_libraries(hamming_test_suite_cluster_plugin SoDAEngine SoDA ${Boost_LIBRARIES})