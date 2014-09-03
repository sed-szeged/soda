project(SoDATools)

include_directories(${SoDATools_SOURCE_DIR}/../../lib/SoDA/inc
                    ${SoDATools_SOURCE_DIR}/../../lib/SoDAPlugin/inc
                    ${SoDATools_SOURCE_DIR}/../../lib/SoDATools/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})
# rawDataReader
add_subdirectory(utilities/rawdatareader)

# reduction
add_subdirectory(utilities/test-suite-reduction)

# test-selection-statistics
add_subdirectory(test-selection-statistics)

# test-selections
add_subdirectory(test-suite-selections)

# test-suite-metrics
add_subdirectory(test-suite-metrics)

# test-suite-score
add_subdirectory(test-suite-score)

# cluster-dump
add_subdirectory(cluster-dump)

# binaryDump
aux_source_directory(${SoDATools_SOURCE_DIR}/utilities/binarydump binaryDump_src)
add_executable(binaryDump ${binaryDump_src})
target_link_libraries(binaryDump SoDA ${Boost_LIBRARIES})

# instrumentServer
aux_source_directory(${SoDATools_SOURCE_DIR}/instrumentserver instrumentServer_src)
add_executable(instrumentServer ${instrumentServer_src})
target_link_libraries(instrumentServer SoDA ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# sample
aux_source_directory(${SoDATools_SOURCE_DIR}/sample sample_src)
add_executable(sample ${sample_src})
target_link_libraries(sample SoDA ${Boost_LIBRARIES})
