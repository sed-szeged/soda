project(dstar_fault_localization_technique_plugin)

include_directories(${dstar_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${dstar_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${dstar_fault_localization_technique_plugin_SOURCE_DIR} dstar_fault_localization_technique_src)

add_library(dstar_fault_localization_technique SHARED ${headers} ${dstar_fault_localization_technique_src})
target_link_libraries(dstar_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
