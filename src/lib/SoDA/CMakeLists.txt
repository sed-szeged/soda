project(SoDALib)

include_directories(${SoDALib_SOURCE_DIR}/inc
                    ${Boost_INCLUDE_DIRS})

#Headers
file(GLOB_RECURSE headers ./inc/*.h)

#Sources
aux_source_directory(${SoDALib_SOURCE_DIR}/src/data data_src)
aux_source_directory(${SoDALib_SOURCE_DIR}/src/exception exception_src)
aux_source_directory(${SoDALib_SOURCE_DIR}/src/interface interface_src)
aux_source_directory(${SoDALib_SOURCE_DIR}/src/io io_src)
aux_source_directory(${SoDALib_SOURCE_DIR}/src src)

add_library(SoDA SHARED ${headers} ${data_src} ${exception_src} ${interface_src} ${io_src} ${src})
