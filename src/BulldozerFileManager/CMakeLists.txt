file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

add_library(BulldozerFileManager SHARED ${SRCS} ${HEADERS})

target_include_directories(BulldozerFileManager PUBLIC "../ExpansionMath")
target_link_libraries(BulldozerFileManager PUBLIC ExpansionMath)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
target_include_directories(BulldozerFileManager PUBLIC ${JSONCPP_INCLUDE_DIRS})
target_link_libraries(BulldozerFileManager PUBLIC ${JSONCPP_LIBRARIES})