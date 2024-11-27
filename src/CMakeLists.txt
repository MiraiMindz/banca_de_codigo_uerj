file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} asan)

