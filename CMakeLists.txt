cmake_minimum_required(VERSION 3.22)
project(setka C)

add_compile_options(-Wall -pedantic -ggdb3)

file(GLOB SOURCES 
    main.c
)

message(STATUS "SOURCES: ${SOURCES}")


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_subdirectory(src)