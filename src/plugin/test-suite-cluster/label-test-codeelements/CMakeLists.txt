project(label_test_codeelements_cluster_plugin)

include_directories(${label_test_codeelements_cluster_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${label_test_codeelements_cluster_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${label_test_codeelements_cluster_plugin_SOURCE_DIR} label_test_codeelements_cluster_plugin_src)

add_library(label_test_codeelements_cluster SHARED ${headers} ${label_test_codeelements_cluster_plugin_src})
target_link_libraries(label_test_codeelements_cluster SoDAEngine SoDA)
