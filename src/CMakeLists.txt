
message(STATUS "SOURCES in child: ${PARENT_SOURCES}")

list(APPEND SOURCES fs.c amsi.c)


add_executable(${PROJECT_NAME} ${SOURCES})
