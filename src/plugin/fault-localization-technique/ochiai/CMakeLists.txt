project(ochiai_fault_localization_technique_plugin)

include_directories(${ochiai_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${ochiai_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${ochiai_fault_localization_technique_plugin_SOURCE_DIR} ochiai_fault_localization_technique_src)

add_library(ochiai_fault_localization_technique SHARED ${headers} ${ochiai_fault_localization_technique_src})
target_link_libraries(ochiai_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
