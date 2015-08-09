project(SoDAEngine)

include_directories(${SoDAEngine_SOURCE_DIR}/../SoDA/inc
                    ${SoDAEngine_SOURCE_DIR}/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

#Headers
file(GLOB_RECURSE headers ./inc/*.h)

aux_source_directory(${SoDAEngine_SOURCE_DIR}/src/engine engine_src)
add_library(SoDAEngine ${LINKING_METHOD} ${headers} ${engine_src})
target_link_libraries(SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS SoDAEngine
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
