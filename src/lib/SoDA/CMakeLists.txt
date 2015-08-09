project(SoDALib)

include_directories(${SoDALib_SOURCE_DIR}/inc
                    ${SoDALib_SOURCE_DIR}/../SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

#Headers
file(GLOB_RECURSE headers ./inc/*.h)

#Sources
aux_source_directory(${SoDALib_SOURCE_DIR}/src/data data_src)
aux_source_directory(${SoDALib_SOURCE_DIR}/src/exception exception_src)
aux_source_directory(${SoDALib_SOURCE_DIR}/src/interface interface_src)
aux_source_directory(${SoDALib_SOURCE_DIR}/src/io io_src)
aux_source_directory(${SoDALib_SOURCE_DIR}/src/util util_src)
aux_source_directory(${SoDALib_SOURCE_DIR}/src src)

add_library(SoDA ${LINKING_METHOD} ${headers} ${data_src} ${exception_src} ${interface_src} ${io_src} ${util_src} ${src})
add_dependencies(SoDA rapidjson)
target_link_libraries(SoDA ${Boost_LIBRARIES})
install(TARGETS SoDA
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
