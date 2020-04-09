cmake_minimum_required(VERSION 3.0)

project(BulldozerFileManager)

include_directories(../ExpansionMath/)

file(GLOB SRC "*.cpp" "*.h")

add_library(BulldozerFileManager SHARED ${SRC})

target_link_libraries(BulldozerFileManager ExpansionMath)

target_include_directories(BulldozerFileManager PUBLIC)
