project(random_reduction_plugin)

include_directories(${random_reduction_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${random_reduction_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${random_reduction_plugin_SOURCE_DIR} random_reduction_src)

add_library(random_reduction SHARED ${headers} ${random_reduction_src})
target_link_libraries(random_reduction SoDAEngine SoDA)
