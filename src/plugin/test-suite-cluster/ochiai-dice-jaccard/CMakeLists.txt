project(ochiai_dice_jaccard_test_suite_cluster_plugin)

include_directories(${ochiai_dice_jaccard_test_suite_cluster_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${ochiai_dice_jaccard_test_suite_cluster_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${ochiai_dice_jaccard_test_suite_cluster_plugin_SOURCE_DIR} ochiai_dice_jaccard_test_suite_cluster_plugin_src)

add_library(ochiai_dice_jaccard_test_suite_cluster_plugin SHARED ${headers} ${ochiai_dice_jaccard_test_suite_cluster_plugin_src})
target_link_libraries(ochiai_dice_jaccard_test_suite_cluster_plugin SoDAEngine SoDA ${Boost_LIBRARIES})
