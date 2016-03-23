project(fl_score)

include_directories(${fl_score_SOURCE_DIR}
                    ${fl_score_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${fl_score_SOURCE_DIR}/../../../lib/SoDAEngine/inc)

file(GLOB fl_score_headers ./*.h)

aux_source_directory(${fl_score_SOURCE_DIR} fl_score_src)

add_executable(fl-score ${fl_score_headers} ${fl_score_src})
target_link_libraries(fl-score SoDAEngine SoDA ${Boost_LIBRARIES} ${LINK_DL})
install(TARGETS fl-score RUNTIME DESTINATION bin)
