project(random_test_suite_cluster_plugin)

include_directories(${random_test_suite_cluster_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${random_test_suite_cluster_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${random_test_suite_cluster_plugin_SOURCE_DIR} random_test_suite_cluster_plugin_src)

add_library(random_test_suite_cluster_plugin SHARED ${headers} ${random_test_suite_cluster_plugin_src})
target_link_libraries(random_test_suite_cluster_plugin SoDAEngine SoDA ${Boost_LIBRARIES})
