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

# coverage
add_subdirectory(coverage)

# test-suite-score
add_subdirectory(test-suite-score)

# test-suite-score
add_subdirectory(mutation-prioritization)

# clustering
add_subdirectory(clustering)

# binaryDump
aux_source_directory(${SoDATools_SOURCE_DIR}/utilities/binarydump binaryDump_src)
add_executable(binaryDump ${binaryDump_src})
target_link_libraries(binaryDump SoDA ${Boost_LIBRARIES})
install(TARGETS binaryDump RUNTIME DESTINATION bin)

# coverage-comparator
aux_source_directory(${SoDATools_SOURCE_DIR}/utilities/coverage-comparator coverageCompare_src)
add_executable(coverage-comparator ${coverageCompare_src})
target_link_libraries(coverage-comparator SoDA ${Boost_LIBRARIES})
install(TARGETS coverage-comparator RUNTIME DESTINATION bin)

# instrumentServer
if (UNIX)
        add_subdirectory(instrumentserver)
        #aux_source_directory(${SoDATools_SOURCE_DIR}/instrumentserver instrumentServer_src)
        #add_executable(instrumentServer ${instrumentServer_src})
        #target_link_libraries(instrumentServer SoDA ${Boost_LIBRARIES})
endif()

# sample
aux_source_directory(${SoDATools_SOURCE_DIR}/sample sample_src)
add_executable(sample ${sample_src})
target_link_libraries(sample SoDA ${Boost_LIBRARIES})
install(TARGETS sample RUNTIME DESTINATION bin)
