project(tarantula_fault_localization_technique_plugin)

include_directories(${tarantula_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${tarantula_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${tarantula_fault_localization_technique_plugin_SOURCE_DIR} tarantula_fault_localization_technique_src)

add_library(tarantula_fault_localization_technique SHARED ${headers} ${tarantula_fault_localization_technique_src})
target_link_libraries(tarantula_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
