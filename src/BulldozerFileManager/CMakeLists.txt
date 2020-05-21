cmake_minimum_required(VERSION 3.0)

project(BulldozerFileManager)

include_directories(../ExpansionMath/)
include_directories(../libs/JSONCPP/include/)

file(GLOB SRC "*.cpp" "*.h")

add_library(BulldozerFileManager SHARED ${SRC})

target_link_libraries(BulldozerFileManager ExpansionMath)

target_include_directories(BulldozerFileManager PUBLIC)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})
target_link_libraries(BulldozerFileManager ${JSONCPP_LIBRARIES})
