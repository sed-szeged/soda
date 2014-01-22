project(SoDAToolsLib)

include_directories(${SoDAToolsLib_SOURCE_DIR}/inc
                    ${SoDAToolsLib_SOURCE_DIR}/../SoDA/inc
                    ${Boost_INCLUDE_DIRS})

#Headers
file(GLOB_RECURSE headers ./inc/*.h)

#Sources
aux_source_directory(${SoDAToolsLib_SOURCE_DIR}/src/datamanager dataManager_src)
aux_source_directory(${SoDAToolsLib_SOURCE_DIR}/src/datareader dataReader_src)
aux_source_directory(${SoDAToolsLib_SOURCE_DIR}/src src)

add_library(SoDATools STATIC ${headers} ${dataManager_src} ${dataReader_src})
