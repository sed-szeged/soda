project(SoDATools)

include_directories(${SoDATools_SOURCE_DIR}/../../lib/SoDA/inc
                    ${SoDATools_SOURCE_DIR}/../../lib/SoDATools/inc
                    ${Boost_INCLUDE_DIRS})

# rawDataReader
aux_source_directory(${SoDATools_SOURCE_DIR}/utilities/rawdatareader rawDataReader_src)
add_executable(rawDataReader ${headers} ${rawDataReader_src})
target_link_libraries(rawDataReader SoDATools SoDA ${Boost_LIBRARIES})

# binaryDump
aux_source_directory(${SoDATools_SOURCE_DIR}/utilities/binarydump binaryDump_src)
add_executable(binaryDump ${headers} ${binaryDump_src})
target_link_libraries(binaryDump SoDATools SoDA ${Boost_LIBRARIES})

# instrumentServer
aux_source_directory(${SoDATools_SOURCE_DIR}/instrumentserver instrumentServer_src)
add_executable(instrumentServer ${instrumentServer_src})
target_link_libraries(instrumentServer SoDA ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# sample
aux_source_directory(${SoDATools_SOURCE_DIR}/sample sample_src)
add_executable(testSelection ${sample_src})
target_link_libraries(testSelection SoDA ${Boost_LIBRARIES})
