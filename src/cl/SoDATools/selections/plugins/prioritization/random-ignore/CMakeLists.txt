project(RandomIgnorePrioritizationPlugin)

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${RandomIgnorePrioritizationPlugin_SOURCE_DIR} random_ignore_prioritization_src)

add_library(random_ignore_prioritization SHARED ${prioritization_plugin_header} ${headers} ${random_ignore_prioritization_src})
target_link_libraries(random_ignore_prioritization SoDA)
