project(GeneralIgnorePrioritizationPlugin)

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${GeneralIgnorePrioritizationPlugin_SOURCE_DIR} general_ignore_prioritization_src)

add_library(general_ignore_prioritization SHARED ${prioritization_plugin_header} ${headers} ${general_ignore_prioritization_src})
target_link_libraries(general_ignore_prioritization SoDA)
