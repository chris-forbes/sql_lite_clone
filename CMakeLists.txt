cmake_minimum_required(VERSION 3.2)
project(furby-db)

include_directories(include)

file(GLOB_RECURSE INCLUDE_FILES ${CMAKE_SOURCE_DIR}/include/*.h)

include_directories(${INCLUDE_FILES})
add_executable(furby-db src/main.c src/Pager.c)
