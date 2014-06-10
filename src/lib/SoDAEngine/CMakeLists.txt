project(SoDAEngine)

include_directories(${SoDAEngine_SOURCE_DIR}/../SoDA/inc
                    ${SoDAEngine_SOURCE_DIR}/inc
                    ${Boost_INCLUDE_DIRS})

#Headers
file(GLOB_RECURSE headers ./inc/*.h)

aux_source_directory(${SoDAEngine_SOURCE_DIR}/src/engine engine_src)

add_library(SoDAEngine SHARED ${headers} ${engine_src})
target_link_libraries(SoDAEngine SoDA ${Boost_LIBRARIES} dl)
