cmake_minimum_required(VERSION 3.2)
project(furby-db)

include_directories(include)

file(GLOB_RECURSE INCLUDE_FILES ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SOURCE_FILES_NO_MAIN ${CMAKE_SOURCE_DIR}/src/*.c)
#list(REMOVE_ITEM SOURCE_FILES_NO_MAIN ${CMAKE_SOURCE_DIR}/src/main.c)
#set(SOURCE_FILES ${SOURCE_FILES_NO_MAIN} ${CMAKE_SOURCE_DIR}/src/main.c)


add_executable(furby-db ${INCLUDE_FILES} ${SOURCE_FILES_NO_MAIN} ${SOURCE_FILE})