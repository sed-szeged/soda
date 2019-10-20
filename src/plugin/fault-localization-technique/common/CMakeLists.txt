project(common_fault_localization_technique_plugin)

include_directories(${common_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${common_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${common_fault_localization_technique_plugin_SOURCE_DIR} common_fault_localization_technique_src)

add_library(common_fault_localization_technique SHARED ${headers} ${common_fault_localization_technique_src})
target_link_libraries(common_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
